
/*CREAZIONE TRIGGER PER CHECK DELLA SPECIALIZZAZIONE DEL PRIMARIO*/
DROP TRIGGER IF EXISTS `ospedale`.`specializzazione_check`;

DELIMITER $$
USE `ospedale`$$
CREATE TRIGGER `specializzazione_check` 
BEFORE UPDATE ON `reparto` 
FOR EACH ROW 
BEGIN
	if((select specializzazione from `personale` where Matricola = new.Primario) <> (select Nome from `reparto` where Id_reparto = new.Id_reparto)) 
		THEN SIGNAL SQLSTATE '45000'
         SET MESSAGE_TEXT ='SPECIALIZZAZIONE DEL DOTTORE NON VALIDA AL FINE DI ESSERE PRIMARIO DI QUESTO REPARTO';
	end if;
END$$
DELIMITER ;

/*Creazione trigger per controllo che la stanza non sia piena*/

DROP TRIGGER IF EXISTS `ospedale`.`ricoveri_BEFORE_INSERT`;

DELIMITER $$
USE `ospedale`$$
CREATE DEFINER=`root`@`localhost` TRIGGER `ricoveri_BEFORE_INSERT` BEFORE INSERT ON `ricoveri` FOR EACH ROW BEGIN
	DECLARE numpazienti INT;
    
    SET numpazienti = (select count(*) as numeropazienti from ospedale.ricoveri where Fine is null and Stanze_NumeroStanza = new.Stanze_NumeroStanza and Stanze_Reparto_Id_reparto = new.Stanze_Reparto_Id_reparto);
    if(numpazienti >= 2) THEN
	SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'ERRORE la stanza che stai cercando di occupare sembra essere piena';
    end if;
END$$
DELIMITER ;

/*Creazione trigger per controllo dimissioni */
DROP TRIGGER IF EXISTS `ospedale`.`ricoveri_BEFORE_UPDATE`;

DELIMITER $$
USE `ospedale`$$
CREATE DEFINER=`root`@`localhost` TRIGGER `ricoveri_BEFORE_UPDATE` BEFORE UPDATE ON `ricoveri` FOR EACH ROW BEGIN
	DECLARE aux INT;
    SET aux = timestampdiff(DAY,new.Inizio,new.Fine);
    IF(aux<0)THEN
		SIGNAL sqlstate '45000' SET MESSAGE_TEXT = "La data di dimissione non può essere prima di quella di ricovero";
	end if;
END$$
DELIMITER ;

/*Un infermiere non può fare due turni lo stesso giorno*/
DROP TRIGGER IF EXISTS `ospedale`.`turni_BEFORE_INSERT`;

DELIMITER $$
USE `ospedale`$$
CREATE DEFINER = CURRENT_USER TRIGGER `ospedale`.`turni_BEFORE_INSERT` BEFORE INSERT ON `turni` FOR EACH ROW
BEGIN
	IF(EXISTS(select * from ospedale.turni where Personale_Matricola = new.Personale_Matricola AND Giorno = new.Giorno))THEN
		SIGNAL SQLSTATE '45000' SET message_text = 'Questo infermiere ha già un turno per quel giorno';
	end if;
END$$
DELIMITER ;

/*Aggiornamento counter posti disponibili in inserimento*/
DROP TRIGGER IF EXISTS `ospedale`.`ricoveri_AFTER_INSERT`;

DELIMITER $$
USE `ospedale`$$
CREATE DEFINER=`root`@`localhost` TRIGGER `ricoveri_AFTER_INSERT` AFTER INSERT ON `ricoveri` FOR EACH ROW BEGIN
	UPDATE ospedale.reparto SET PostiDisponibili = PostiDisponibili - 1 WHERE Id_reparto = new.Stanze_Reparto_Id_reparto;
END$$
DELIMITER ;


/*Aggiornamento counter posti disponibili in update della fine*/
DROP TRIGGER IF EXISTS `ospedale`.`ricoveri_AFTER_UPDATE`;

DELIMITER $$
USE `ospedale`$$
CREATE DEFINER = CURRENT_USER TRIGGER `ospedale`.`ricoveri_AFTER_UPDATE` AFTER UPDATE ON `ricoveri` FOR EACH ROW
BEGIN
	if(old.Fine IS NULL AND new.Fine IS NOT NULL) THEN
		UPDATE ospedale.reparto SET PostiDisponibili = PostiDisponibili + 1 WHERE Id_reparto = new.Stanze_Reparto_Id_reparto;
	end if;
END;$$
DELIMITER ;

/*Non si possono effettuare più di 5 visite in reparto lo stesso giorno*/
DROP TRIGGER IF EXISTS `ospedale`.`visite_BEFORE_INSERT`;

DELIMITER $$
USE `ospedale`$$
CREATE DEFINER=`root`@`localhost` TRIGGER `visite_BEFORE_INSERT` BEFORE INSERT ON `visite` FOR EACH ROW BEGIN
	DECLARE visite INT;
    SET visite = (select count(*) as visiteeffettuate from ospedale.visite where Reparto_Id_reparto = new.Reparto_Id_reparto and Giorno = new.Giorno);
    if visite>=5 then
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Non possono più essere effettuate visite in questo reparto';
	end if;
END$$
DELIMITER ;

