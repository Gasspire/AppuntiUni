CREATE SCHEMA IF NOT EXISTS `ospedale` ;

CREATE TABLE IF NOT EXISTS `ospedale`.`Paziente` (
  `CodiceFiscale` VARCHAR(16) NOT NULL,
  `Nome` VARCHAR(45) NULL,
  `Cognome` VARCHAR(45) NULL,
  `Indirizzo` VARCHAR(45) NULL,
  `NumerodiTelefono` INT NULL,
  PRIMARY KEY (`CodiceFiscale`));

CREATE TABLE IF NOT EXISTS `ospedale`.`CartellaClinica` (
  `Paziente_CodiceFiscale` VARCHAR(16) NOT NULL,
  `Genere` ENUM("M", "F") NULL,
  `DatadiNascita` DATETIME NULL,
  `Allergie` VARCHAR(45) NULL,
  `Intolleranze` VARCHAR(45) NULL,
  `MalattiePregresse` VARCHAR(45) NULL,
  PRIMARY KEY (`Paziente_CodiceFiscale`),
  FOREIGN KEY (`Paziente_CodiceFiscale`) REFERENCES `ospedale`.`Paziente` (`CodiceFiscale`));
   
CREATE TABLE IF NOT EXISTS `ospedale`.`Personale` (
  `Matricola` INT NOT NULL,
  `Nome` VARCHAR(45) NULL,
  `Cognome` VARCHAR(45) NULL,
  `Telefono` INT NULL,
  `Ruolo` ENUM("Dottore", "Infermiere", "Amministrazione") NOT NULL,
  `Genere` ENUM("M", "F") NULL,
  `Specializzazione` VARCHAR(45) NULL,
  `PreferenzaTurno` ENUM("Mattina", "Pomeriggio", "Sera") NULL,
  PRIMARY KEY (`Matricola`));

CREATE TABLE IF NOT EXISTS `ospedale`.`PianodiCura` (
  `Paziente_CodiceFiscale` VARCHAR(16) NOT NULL,
  `Terapie` VARCHAR(45) NULL,
  `Farmaci` VARCHAR(45) NULL,
  `MedicodiRiferimento` INT NOT NULL,
  PRIMARY KEY (`Paziente_CodiceFiscale`),
  FOREIGN KEY (`Paziente_CodiceFiscale`) REFERENCES `ospedale`.`Paziente` (`CodiceFiscale`),
  FOREIGN KEY (`MedicodiRiferimento`) REFERENCES `ospedale`.`Personale` (`Matricola`));

CREATE TABLE IF NOT EXISTS `ospedale`.`Reparto` (
  `Id_reparto` INT NOT NULL,
  `Nome` VARCHAR(45) NULL,
  `PostiTotali` VARCHAR(45) NULL,
  `PostiDisponibili` VARCHAR(45) NULL,
  `Primario` INT NULL,
  PRIMARY KEY (`Id_reparto`));

CREATE TABLE IF NOT EXISTS `ospedale`.`Stanze` (
  `NumeroStanza` INT NOT NULL,
  `Reparto_Id_reparto` INT NOT NULL,
  PRIMARY KEY (`NumeroStanza`, `Reparto_Id_reparto`),
  FOREIGN KEY (`Reparto_Id_reparto`) REFERENCES `ospedale`.`Reparto` (`Id_reparto`));



CREATE TABLE IF NOT EXISTS `ospedale`.`Turni` (
  `Giorno` DATE NOT NULL,
  `Orario` ENUM("Mattina", "Pomeriggio", "Sera") NOT NULL,
  `Personale_Matricola` INT NOT NULL,
  PRIMARY KEY (`Giorno`, `Orario`, `Personale_Matricola`),
  FOREIGN KEY (`Personale_Matricola`)  REFERENCES `ospedale`.`Personale` (`Matricola`));

CREATE TABLE IF NOT EXISTS `ospedale`.`Visite` (
  `Paziente_CodiceFiscale` VARCHAR(16) NOT NULL,
  `Reparto_Id_reparto` INT NOT NULL,
  `Giorno` DATE NULL,
  `Esito` VARCHAR(45) NULL,
  PRIMARY KEY (`Paziente_CodiceFiscale`, `Reparto_Id_reparto`),
  FOREIGN KEY (`Paziente_CodiceFiscale`) REFERENCES `ospedale`.`Paziente` (`CodiceFiscale`),
  FOREIGN KEY (`Reparto_Id_reparto`) REFERENCES `ospedale`.`Reparto` (`Id_reparto`));


CREATE TABLE IF NOT EXISTS `ospedale`.`Ricoveri` (
  `Paziente_CodiceFiscale` VARCHAR(16) NOT NULL,
  `Stanze_NumeroStanza` INT NOT NULL,
  `Stanze_Reparto_Id_reparto` INT NOT NULL,
  `Inizio` DATE NOT NULL,
  `Fine` DATE NULL,
  PRIMARY KEY (`Paziente_CodiceFiscale`, `Stanze_NumeroStanza`, `Stanze_Reparto_Id_reparto`,`Inizio`),
  FOREIGN KEY (`Paziente_CodiceFiscale`) REFERENCES `ospedale`.`Paziente` (`CodiceFiscale`),
  FOREIGN KEY (`Stanze_NumeroStanza` , `Stanze_Reparto_Id_reparto`) REFERENCES `ospedale`.`Stanze` (`NumeroStanza` , `Reparto_Id_reparto`));


CREATE TABLE IF NOT EXISTS `ospedale`.`AffiliazioneReparto` (
  `Reparto_Id_reparto` INT NOT NULL,
  `Personale_Matricola` INT NOT NULL,
  PRIMARY KEY (`Reparto_Id_reparto`, `Personale_Matricola`),
  FOREIGN KEY (`Reparto_Id_reparto`) REFERENCES `ospedale`.`Reparto` (`Id_reparto`),
  FOREIGN KEY (`Personale_Matricola`) REFERENCES `ospedale`.`Personale` (`Matricola`));


