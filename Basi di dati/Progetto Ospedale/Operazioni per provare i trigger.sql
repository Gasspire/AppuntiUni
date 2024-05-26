/*Per controllare il trigger della validità del turno dell'infermiere*/
INSERT INTO ospedale.turni (`Giorno`,`Orario`,`Personale_Matricola`)
VALUES ("2020-10-10","Pomeriggio","101");

INSERT INTO ospedale.turni (`Giorno`,`Orario`,`Personale_Matricola`)
VALUES ("2020-10-10","Mattino","101");

/*Per controllare il trigger della specializzazione del dottore*/
UPDATE `ospedale`.`reparto` SET `Primario` = '1' WHERE (`Id_reparto` = '1000');
UPDATE `ospedale`.`reparto` SET `Primario` = '7' WHERE (`Id_reparto` = '1000');

/*Per controllare il trigger delle visite in reparto*/
INSERT INTO `ospedale`.`visite` (`Paziente_CodiceFiscale`, `Reparto_Id_reparto`, `Giorno`) VALUES ('ABCD123E45F6789G', '1000', '2020-10-10');
INSERT INTO `ospedale`.`visite` (`Paziente_CodiceFiscale`, `Reparto_Id_reparto`, `Giorno`) VALUES ('ABCDEF12G34H567I', '1000', '2020-10-10');
INSERT INTO `ospedale`.`visite` (`Paziente_CodiceFiscale`, `Reparto_Id_reparto`, `Giorno`) VALUES ('BCDEF90G12H345IG', '1000', '2020-10-10');
INSERT INTO `ospedale`.`visite` (`Paziente_CodiceFiscale`, `Reparto_Id_reparto`, `Giorno`) VALUES ('BCDEFG90H12I345J', '1000', '2020-10-10');
INSERT INTO `ospedale`.`visite` (`Paziente_CodiceFiscale`, `Reparto_Id_reparto`, `Giorno`) VALUES ('CDEFGH56I78J910K', '1000', '2020-10-10');


INSERT INTO `ospedale`.`visite` (`Paziente_CodiceFiscale`, `Reparto_Id_reparto`, `Giorno`) VALUES ('DEFGHI34J56K789L', '1000', '202020-10-10');


/*Per controllare il trigger che gestisce le disponibilità di ricoveri*/
INSERT INTO `ospedale`.`ricoveri` (`Paziente_CodiceFiscale`, `Stanze_NumeroStanza`, `Stanze_Reparto_Id_reparto`, `Inizio`) VALUES ('DEFGHI34J56K789L', '1', '1000', '2020-10-10');
UPDATE `ospedale`.`ricoveri` SET `Fine` = '2020-10-30' WHERE (`Paziente_CodiceFiscale` = 'DEFGHI34J56K789L') and (`Stanze_NumeroStanza` = '1') and (`Stanze_Reparto_Id_reparto` = '1000') and (`Inizio` = '2020-10-10');

/*Per controllare il trigger della validità della data*/
UPDATE `ospedale`.`ricoveri` SET `Fine` = '2010-10-30' WHERE (`Paziente_CodiceFiscale` = 'DEFGHI34J56K789L') and (`Stanze_NumeroStanza` = '1') and (`Stanze_Reparto_Id_reparto` = '1000') and (`Inizio` = '2020-10-10');

/*Per controllare il trigger di inserimento in una stanza piena*/
INSERT INTO `ospedale`.`ricoveri` (`Paziente_CodiceFiscale`, `Stanze_NumeroStanza`, `Stanze_Reparto_Id_reparto`, `Inizio`, `Fine`) VALUES ('BCDEFG90H12I345J', '2', '1000', '2020-10-11', '2020-11-30');
INSERT INTO `ospedale`.`ricoveri` (`Paziente_CodiceFiscale`, `Stanze_NumeroStanza`, `Stanze_Reparto_Id_reparto`, `Inizio`) VALUES ('BCDEFG90H12I345J', '2', '1000', '2023-11-11');
INSERT INTO `ospedale`.`ricoveri` (`Paziente_CodiceFiscale`, `Stanze_NumeroStanza`, `Stanze_Reparto_Id_reparto`, `Inizio`) VALUES ('KLMNOP12Q34R567S', '2', '1000', '2023-11-15');

INSERT INTO `ospedale`.`ricoveri` (`Paziente_CodiceFiscale`, `Stanze_NumeroStanza`, `Stanze_Reparto_Id_reparto`, `Inizio`) VALUES ('MNOPQR56S78T910U', '2', '1000', '2024-02-28');

