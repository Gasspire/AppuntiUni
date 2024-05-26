INSERT INTO ospedale.paziente (`CodiceFiscale`,`Nome`,`Cognome`,`Indirizzo`,`NumerodiTelefono`)
VALUES 
	("ABCDEF12G34H567I","Marco","Rossi","Via Roma 1","0123456789"),
	("DEFGHI34J56K789L","Giulia","Bianchi","Via Verdi 2","0234567890"),
	("MNOPQR56S78T910U","Alessandro","Verdi","Via Garibaldi 3","0345678901"),
	("STUVWX78Y90Z123A","Luca","Ferrari","Via Mazzini 4","0456789012"),
	("BCDEFG90H12I345J","Sara","Romano","Via Dante 5","0567890123"),
	("KLMNOP12Q34R567S","Marta","Conti","Via Manzoni 6","0678901234"),
	("TUVWXY34Z56A789B","Giovanni","Galli","Via Leopardi 7","0789012345"),
	("CDEFGH56I78J910K","Anna","Moretti","Via Volta 8","0890123456"),
	("LMNOPQ78R90S123T","Federico","Colombo","Via Vittorio Emanuele 9","0901234567"),
	("UVWXYZ90A12B345C","Elisa","Ferrara","Via Carducci 10","0012345678"),
	("EFGHIJ12K34L567M","Mattia","Marini","Via Galileo 11","0123456789"),
	("PQRSTUV34W56X789","Marta","Rossi","Via Michelangelo 12","0234567890"),
	("ABCD123E45F6789G","Paolo","Riva","Via Leonardo 13","0345678901"),
	("HIJKLM56N78O910P","Simone","Fontana","Via Monti 14","0456789012"),
	("QRSTUVWXYZ78V90W","Alice","Barbieri","Via Colombo 15","0567890123"),
	("BCDEF90G12H345IG","Michele","De Luca","Via Donizetti 16","0678901234");
    
INSERT INTO ospedale.cartellaclinica (`Paziente_CodiceFiscale`,`Genere`,`DatadiNascita`,`Allergie`,`Intolleranze`,`MalattiePregresse`)
VALUES 
	("ABCDEF12G34H567I","M","1980-05-15","Polleni","Glutine","Nessuna"),
	("DEFGHI34J56K789L","F","1992-11-28","Acari","Lattosio","Nessuna"),
	("MNOPQR56S78T910U","M","1975-08-03","Polvere","Noci","Ipertensione"),
	("STUVWX78Y90Z123A","M","1988-03-20","Polline","Uova","Asma"),
	("BCDEFG90H12I345J","F","1987-12-10","Peluria Animali","Pesce","Diabete"),
	("KLMNOP12Q34R567S","F","1995-06-25","Muffe","Soia","Nessuna"),
	("TUVWXY34Z56A789B","M","1982-09-08","Polline","Crostacei","Nessuna"),
	("CDEFGH56I78J910K","F","1978-04-17","Acari","Arachidi","Nessuna"),
	("LMNOPQ78R90S123T","M","1973-07-29","Polvere","Sesamo","Colesterolo alto"),
	("UVWXYZ90A12B345C","F","1984-02-14","Polleni","Latte","Nessuna"),
	("EFGHIJ12K34L567M","M","1990-10-03","Peluria Animali","Glutine","Nessuna"),
	("PQRSTUV34W56X789","F","1983-11-19","Muffe","Uova","Nessuna"),
	("ABCD123E45F6789G","M","1976-06-07","Polline","Soia","Nessuna"),
	("HIJKLM56N78O910P","M","1989-09-22","Peluria Animali","Arachidi","Nessuna"),
	("QRSTUVWXYZ78V90W","F","1981-04-05","Polvere","Pesce","Nessuna"),
	("BCDEF90G12H345IG","M","1997-01-30","Muffe","Crostacei","Nessuna");

INSERT INTO ospedale.personale (`Matricola`,`Nome`,`Cognome`,`Telefono`,`Ruolo`,`Genere`,`Specializzazione`,`PreferenzaTurno`)
VALUES
	("1","Alessandro","Verdi","0345678901","Dottore","M","Chirurgia generale",null),
	("7","Luca","Ferrari","0456789012","Dottore","M","Cardiologia",null),
	("100","Sara","Romano","0567890123","Infermiere","F",null,"Pomeriggio"),
	("101","Marta","Conti","0678901234","Infermiere","F",null,"Sera"),
	("102","Giovanni","Galli","0789012345","Infermiere","M",null,"Mattina"),
	("10","Anna","Moretti","0890123456","Dottore","F","Oncologia",null),
	("15","Federico","Colombo","0901234567","Dottore","M","Dermatologia",null),
	("103","Elisa","Ferrara","0012345678","Infermiere","F",null,"Mattina"),
	("104","Mattia","Marini","0123456789","Infermiere","M",null,"Pomeriggio"),
	("105","Marta","Rossi","0234567890","Infermiere","F",null,"Sera"),
	("106","Paolo","Riva","0345678901","Infermiere","M",null,"Mattina"),
	("107","Simone","Fontana","0456789012","Infermiere","M",null,"Pomeriggio"),
	("17","Alice","Barbieri","0567890123","Dottore","F","Radiologia",null),
	("108","Michele","De Luca","0678901234","Infermiere","M",null,"Mattina"),
	("50","Sofia","Romano","0789012345","Amministrazione","F",null,null),
	("51","Andrea","Conti","0890123456","Amministrazione","M",null,null),
	("52","Francesca","Marini","0901234567","Amministrazione","F",null,null),
	("53","Luigi","Rossi","0012345678","Amministrazione","M",null,null);
    
INSERT INTO ospedale.pianodicura (`Paziente_CodiceFiscale`,`Terapie`,`Farmaci`,`MedicodiRiferimento`)
VALUES
	("ABCDEF12G34H567I","Terapia fisica","Aspirina",10),
	("DEFGHI34J56K789L","Psicoterapia","Paracetamolo","10"),
	("MNOPQR56S78T910U","Fisioterapia","Ibuprofene","10"),
	("STUVWX78Y90Z123A","Logopedia","Morfina","10"),
	("BCDEFG90H12I345J","Chiropratica","Antibiotici","15"),
	("KLMNOP12Q34R567S","Osteopatia","Prozac","15"),
	("TUVWXY34Z56A789B","Massaggio","Ranitidina","15"),
	("CDEFGH56I78J910K","Psicoterapia","Atorvastatina","15"),
	("LMNOPQ78R90S123T","Fisioterapia","Omeprazolo","10"),
	("UVWXYZ90A12B345C","null","Mirtazapina","7"),
	("EFGHIJ12K34L567M","Terapia fisica","Lisinopril","7"),
	("PQRSTUV34W56X789","null","Valsartan","7"),
	("ABCD123E45F6789G","Chiropratica","Warfarin","1"),
	("HIJKLM56N78O910P","Logopedia","Insulina","1"),
	("QRSTUVWXYZ78V90W","null","Fluoxetina","1"),
	("BCDEF90G12H345IG","Psicoterapia","Clonazepam","17");

INSERT INTO ospedale.reparto (`Id_reparto`,`Nome`,`PostiTotali`,`PostiDisponibili`,`Primario`)
values
	("1000","Cardiologia",30,15,"7"),
	("2000","Pediatria",30,10,null),
	("3000","Ortopedia",30,12,null),
	("4000","Oncologia",30,20,"10"),
	("5000","Radiologia",30,25,"17"),
	("6000","Gastroenterologia",30,18,null);
    
INSERT INTO ospedale.stanze (`NumeroStanza`,`Reparto_Id_reparto`)
VALUES 
	('1',1000),('2',1000),('3',1000),('4',1000),('5',1000),('6',1000),('7',1000),('8',1000),
    ('1',2000),('2',2000),('3',2000),('4',2000),('5',2000),('6',2000),('7',2000),('8',2000),
	('1',3000),('2',3000),('3',3000),('4',3000),('5',3000),('6',3000),('7',3000),('8',3000),
    ('1',4000),('2',4000),('3',4000),('4',4000),('5',4000),('6',4000),('7',4000),('8',4000),
    ('1',5000),('2',5000),('3',5000),('4',5000),('5',5000),('6',5000),('7',5000),('8',5000),
    ('1',6000),('2',6000),('3',6000),('4',6000),('5',6000),('6',6000),('7',6000),('8',6000);
    
    
INSERT INTO ospedale.affiliazionereparto (`Reparto_Id_reparto`,`Personale_Matricola`)
VALUES
	(1000,7),(4000,10),(5000,17),(2000,1),(3000,15),(6000,50),(1000,51),(2000,52),(3000,53),
    (4000,100),(5000,101),(6000,102),(1000,103),(2000,104),(3000,105),(4000,106),(5000,107),
    (6000,108)