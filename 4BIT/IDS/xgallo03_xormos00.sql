/**
 *  IDS Projekt c.4
 *  SQL skript pre vytvorenie zakladnych objektov schematu databaze
 *
 *  24.4.2016
 *  Lubomir Gallovic, 	xgallo03@stud.fit.vutbr.cz
 *  Michal Ormos, 	    xormos00@stud.fit.vutbr.cz
 */

-- Vymazanie tabuliek s danymi nazvami, aby vytvorili miesto pre nove
DROP TABLE zamestnanec cascade constraints;
DROP TABLE liecba cascade constraints;
DROP TABLE zviera cascade constraints;
DROP TABLE zivocisny_druh cascade constraints;
DROP TABLE majitel cascade constraints;
DROP TABLE liek cascade constraints;
DROP TABLE davkovanie cascade constraints;
DROP TABLE zaobstara cascade constraints;
DROP TABLE vyuziva cascade constraints;
DROP SEQUENCE pocetID;

--------------------------------------------------------------------------------
------------------------VYTVORENIE TABULIEK PROJEKT 2---------------------------
--------------------NASTAVENIE PRIMARNYCH KLUCOV PROJEKT 2----------------------
---------------------NASTAVENIE CUDZICH KLUCOV PROJEKT 2------------------------
--------------------------------------------------------------------------------
create table zamestnanec (
  rodne_cislo int not null,  --presny pocet cisel
  meno char(20) not null,
  priezvisko char(20) not null,
  mesto char(20),
  ulica char(20) not null,
  psc int,
  cislo_uctu varchar(10) not null,
  hodinova_mzda int not null,
  typ char(10) not null check (typ='Veterinar' or typ='Sestra'),
  titul char(6),
  specializacia char (20),
  skola char (20),
  check(round(rodne_cislo/11.0) = rodne_cislo/11.0),
  -- validne rodne cisla musia byt delitelne 11
  -- ludia narodeny skor ako 1.1.1954 maju smolu
  -- demonstracia ukazky prace s constraint
  primary key (rodne_cislo)
);

create table zivocisny_druh (
  meno_druhu varchar(20) not null,
  primary key (meno_druhu)
);

create table majitel (
  cislo_op varchar(8) not null,
  meno char(20) not null,
  priezvisko char(20) not null,
  mesto char(20),
  ulica char(20) not null,
  psc int,
  primary key (cislo_op)
);

create table zviera (
  idef_cislo int not null,
  meno char(20) not null,
  datum_narodenia int not null,
  datum_poslednej_prehliadky int not null,
  meno_druhu varchar(20) not null,
  cislo_op varchar(8) not null,
  primary key (idef_cislo),
  foreign key (meno_druhu) references zivocisny_druh,
  foreign key (cislo_op) references majitel
);

create table liek (
  nazov varchar(40) not null,
  typ_lieku varchar(100) not null,
  ucinna_latka varchar(50 ) not null,
  kontraindikacia varchar(30) not null,
  primary key (nazov)
);

create table liecba (
  cislo_liecby varchar(10) not null,
  diagnoza varchar(50) not null,
  datum_zahajenie_liecby int not null,
  stav char(100),
  cena int not null,
  vypisal_meno_priezvisko char(40) not null,
  idef_cislo int not null,
  primary key (cislo_liecby),
foreign key (idef_cislo) references zviera
);

create table davkovanie (
  nazov varchar(40) not null,
  meno_druhu varchar(40) not null,
  odporucana_davka varchar(50) not null,
  doba_podavania varchar(50),
  primary key (nazov, meno_druhu),
  foreign key (nazov) references liek on delete cascade,
  foreign key (meno_druhu) references zivocisny_druh on delete cascade
);

create table vyuziva (
  cislo_liecby varchar(10),
  nazov varchar(40),
  doba_davkovania varchar(50),
  podavanie varchar(50),
  primary key (cislo_liecby, nazov),
  foreign key(cislo_liecby) references liecba on delete cascade,
  foreign key(nazov) references liek on delete cascade
);

create table zaobstara (
  cislo_liecby varchar(10),
  rodne_cislo int,
  primary key (cislo_liecby, rodne_cislo),
  foreign key(cislo_liecby) references liecba on delete cascade,
  foreign key(rodne_cislo) references zamestnanec on delete cascade
);

--------------------------------------------------------------------------------
---------------------------PROCEDURY PROJEKT4-----------------------------------
--------------------------------------------------------------------------------
--vypocita priemernu cenu liecby pre danu diagnozu pre dany zivocisny druh
SET serveroutput ON;
CREATE OR REPLACE PROCEDURE priemerna_cena(typ_diagnozy IN liecba.diagnoza%TYPE, typ_druhu IN zviera.meno_druhu%TYPE)
IS
  suma NUMBER;
  pocet NUMBER;
  cursor liecba_druhu IS 
      SELECT L.* 
      FROM liecba L, zviera Z
      WHERE L.idef_cislo=Z.idef_cislo and Z.meno_druhu=typ_druhu; 
  liecba_druhu_row liecba_druhu%ROWTYPE;

BEGIN
  suma:=0;
  pocet:=0;
  OPEN liecba_druhu;
  LOOP
    FETCH liecba_druhu into liecba_druhu_row;
    EXIT WHEN liecba_druhu%notfound;
    IF (liecba_druhu_row.diagnoza = typ_diagnozy) THEN
      suma:=suma+liecba_druhu_row.cena;
      pocet:=pocet+1;
    END IF;
  END LOOP;
  dbms_output.put_line('Priemerna cena diagnozy ' || typ_diagnozy || ' pre zivocisny druh ' || typ_druhu || ' je ' || suma/pocet || ' EUR.');
  EXCEPTION
    WHEN ZERO_DIVIDE THEN
      dbms_output.put_line('Neboly najdene ziadne zaznamy o danej liecbe pre dany zivocisny druh');
  CLOSE liecba_druhu;
END;
/
--vypocita kolko dany majitel utratil na liecby a liecby ktoreho jeho zvierata ho stali najviac
SET serveroutput ON;
CREATE OR REPLACE PROCEDURE vydaje_majitela(cislo_majitela IN majitel.cislo_op%TYPE)
IS
  suma_max NUMBER;
  suma_zviera NUMBER;
  suma_spolu NUMBER;
  sucasne_zviera liecba.idef_cislo%TYPE;
  first_iter NUMBER;
  zviera_najviac liecba.idef_cislo%TYPE;
  meno_naj_zvierata zviera.meno%TYPE;
  meno_majitela majitel.meno%TYPE;
  priezvisko_majitela majitel.priezvisko%TYPE;
  cursor liecby_zvierat IS 
      SELECT L.* 
      FROM zviera Z, liecba L
      WHERE Z.cislo_op=cislo_majitela and L.idef_cislo=Z.idef_cislo
      ORDER BY L.idef_cislo;
  liecby_zvierat_row liecby_zvierat%ROWTYPE;

BEGIN
  suma_zviera:=0;
  suma_spolu:=0;
  first_iter:=0;
  suma_max:=0;
  OPEN liecby_zvierat;
  LOOP
    FETCH liecby_zvierat into liecby_zvierat_row;
    EXIT WHEN liecby_zvierat%notfound;
    IF (first_iter = 0) THEN -- pri prvej polozke sa nastavi prve zviera
      sucasne_zviera:=liecby_zvierat_row.idef_cislo;
      first_iter:=1;
    END IF;

    suma_spolu:=suma_spolu + liecby_zvierat_row.cena;

    IF (liecby_zvierat_row.idef_cislo=sucasne_zviera) THEN -- ak stale idu liecby sucasneho zvierata, pripocita sa cena
      suma_zviera:=suma_zviera+liecby_zvierat_row.cena;
    END IF;
    IF (liecby_zvierat_row.idef_cislo!=sucasne_zviera) THEN -- ak nie, zmeni sa zviera a porovna sa ci je za dane zviera utratene viac
      IF (suma_zviera>suma_max) THEN
        zviera_najviac:=liecby_zvierat_row.idef_cislo;
        suma_max:=suma_zviera;
      END IF;
      sucasne_zviera:=liecby_zvierat_row.idef_cislo;
      suma_zviera:=liecby_zvierat_row.cena;
    END IF;
    IF (suma_zviera>suma_max) THEN
      zviera_najviac:=liecby_zvierat_row.idef_cislo;
      suma_max:=suma_zviera;
    END IF;

  END LOOP;

  SELECT Z.meno INTO meno_naj_zvierata FROM zviera Z WHERE idef_cislo=zviera_najviac;
  SELECT M.meno INTO meno_majitela FROM majitel M WHERE cislo_op=cislo_majitela;
  SELECT M.priezvisko INTO priezvisko_majitela FROM majitel M WHERE cislo_op=cislo_majitela;

  dbms_output.put_line('Majitel ' || meno_majitela || ' ' || priezvisko_majitela || ' utratil za liecby ' || suma_spolu || ' EUR.');
  dbms_output.put_line('Najviac stali liecby zvierata ' || meno_naj_zvierata);
  EXCEPTION
    WHEN NO_DATA_FOUND THEN
      dbms_output.put_line('Majitel s danym identifikacnym cislom neexistje');
  CLOSE liecby_zvierat;
END;
/
--------------------------------------------------------------------------------
------------------------VKLADANIE DO TABULIEK PROJEKT 2-------------------------
--------------------------------------------------------------------------------

insert into zamestnanec
VALUES(8405061236, 'Matej', 'Dzurov', 'Brno', 'Kolejni 2', 61200, '1234567890', 5, 'Veterinar', 'MVDR.', 'kone', NULL); 
insert into zamestnanec
VALUES(9411190162, 'Matej', 'Bielik', 'Bratislava', 'Alstrova 3', 83106, '9133519265', 6, 'Veterinar', 'MVDR.', 'psy', NULL); 
insert into zamestnanec
VALUES(8962161681, 'Jana', 'Veselá', 'Bratislava', 'Bulharská 7', 82104, '9438805999', 3, 'Sestra', NULL, NULL, 'Sv. Alžbety'); 
insert into zamestnanec
VALUES(9110088889, 'Tatiana', 'Kováčová', 'Bratislava', 'Galvaniho 12', 82104, '5200021323', 3, 'Sestra', NULL, NULL, 'Sv. Bazila Veľkého'); 
insert into zamestnanec
VALUES(9608065049, 'Stanislav', 'Lipský', 'Bratislava', 'Janáčkova 26', 81108, '1692202787', 5, 'Veterinar', 'MVDR.', 'ovce', NULL); 
insert into zamestnanec
VALUES(9358178984, 'Martin', 'Kráľ', 'Bratislava', 'Kapitulská 2', 81101, '7958171253', 4, 'Veterinar', 'MVDR.', 'mačky', NULL); 
insert into zamestnanec
VALUES(8953041020, 'Ema', 'Flašíková', 'Dunajská Lužná', 'Lipnická 18', 90042, '5055987544', 3, 'Sestra', NULL, NULL, 'Sv. Bazila Veľkého'); 
insert into zamestnanec
VALUES(9103123920, 'Mária', 'Baranová', 'Malinovo', 'Vŕbová 10', 90045, '3496746838', 3, 'Sestra', NULL, NULL, 'Sv. Alžbety'); 

insert into zivocisny_druh
VALUES('pes domáci');
insert into zivocisny_druh
VALUES('mačka domáca');
insert into zivocisny_druh
VALUES('kôň domáci');
insert into zivocisny_druh
VALUES('ovca domáca');
insert into zivocisny_druh
VALUES('morča domáce');
insert into zivocisny_druh
VALUES('užovka červená');

insert into majitel
VALUES('EA123456', 'Ružena', 'Bayerová', 'Bratislava', 'Kočánkova 6', 85104);
insert into majitel
VALUES('EA521746', 'Peter', 'Blaho', 'Bratislava', 'Májová 8', 85101);
insert into majitel
VALUES('EA136356', 'Peter', 'Leškovský', 'Bratislava', 'Pražská 28', 81104);
insert into majitel
VALUES('EA902168', 'Marta', 'Pavlovičová', 'Bratislava', 'Laurinská 36', 81101);
insert into majitel
VALUES('EA953393', 'Katarína', 'Valová', 'Bratislava', 'Strakova 2', 81101);
insert into majitel
VALUES('EA382384', 'Stanislav', 'Bauer', 'Nitra', 'Kolmá 14', 85101);
insert into majitel
VALUES('EA977088', 'Ľubica', 'Floriánová', 'Pezinok', 'Narcisová 18', 82101);
insert into majitel
VALUES('EA698156', 'Peter', 'Rázus', 'Banksá bystrica', 'SNP 32', 97401);
insert into majitel
VALUES('EA432490', 'Peter', 'Roškovič', 'Nitra', 'Matice slovenskej 15', 95145);

insert into zviera
VALUES('000000001', 'Majo', 20000101, 20150101, 'pes domáci', 'EA123456');
insert into zviera
VALUES('000000002', 'Fufo', 20030412, 20150102, 'mačka domáca', 'EA123456');
insert into zviera
VALUES('000000003', 'Niko', 20071125, 20150103, 'morča domáce', 'EA977088');
insert into zviera
VALUES('000000004', 'Jojo', 20090224, 20150104, 'užovka červená', 'EA382384');
insert into zviera
VALUES('459638524', 'Alex', 19990728, 20150105, 'ovca domáca', 'EA902168');
insert into zviera
VALUES('785103259', 'Hexo', 20060613, 20150106, 'pes domáci', 'EA136356');
insert into zviera
VALUES('156389524', 'Rocko', 20061115, 20150107, 'morča domáce', 'EA521746');
insert into zviera
VALUES('465379857', 'Tina', 20031109, 20150108, 'kôň domáci', 'EA123456');
insert into zviera
VALUES('243265789', 'Dunco', 20060907, 20140809, 'pes domáci', 'EA698156');
insert into zviera
VALUES('139043890', 'Drogo', 20031112, 20140809, 'pes domáci', 'EA698156');
insert into zviera
VALUES('234554890', 'Asta', 20040125, 20150108, 'pes domáci', 'EA432490');

insert into liek
values('Alavis5', 'Pre regeneráciu a obnovu klbových chrupaviek a tkaniva', 'Glukosamin sulfát', 'pre psov a mačky');
insert into liek
values('Celadrin', 'koňom s osteoartrózou', 'Celadrin, maltodextrín', 'pre kone');
insert into liek
values('SUPREME SCIENCE Selective', 'proti zapachu morciat', 'Lucerna, pšenica, sójové otruby', 'Morča ');
insert into liek
values('Paralen', 'univerzalny proti bolesti', 'Lucerna', 'pre vsetky druhy zvierat');

insert into liecba
VALUES('0000000001', 'Zotavenie po ukusnuti', 20160323, 'Lieky proti bolesti', 10, 'Matej Dzurov','000000004');
insert into liecba
VALUES('1256984036', 'Nevladnost, mdlobost', 20160108, 'Domace pozorovanie', 5, 'Matej Bielik','459638524');
insert into liecba
VALUES('4569823659', 'Problemy s klbom', 20160215, 'Nasadenie liekov na klby pre psi a macky', 12, 'Stanislav Lipský','000000001');
insert into liecba
VALUES('4123569871', 'Zlomena noha', 20160129, 'Sadra', 120, 'Martin Kráľ','156389524');
insert into liecba
VALUES('1368958258', 'Rozhriznuty chvost po bitke', 20160315, 'Lieky proti bolesti', 10, 'Matej Dzurov','000000002');
insert into liecba
VALUES('2828722872', 'Zlomena noha', 20160213, 'Sadra', 120, 'Stanislav Lipský','139043890');
insert into liecba
VALUES('2782080287', 'Rozhriznute ucho', 20160208, 'Lieky proti bolesti', 12, 'Matej Bielik','459638524');
insert into liecba
VALUES('2082782872', 'Zlomena noha', 20160115, 'Sadra zacina sa liecit', 150, 'Martin Kráľ','243265789');
insert into liecba
VALUES('2872872870', 'Privelky nevysvetlitelny zapach', 20160321, 'Lieky pre morcata a zapach', 20, 'Matej Bielik','000000003');
insert into liecba
VALUES('3959430842', 'Samovolne vypadavanie vlny', 20150908, 'Domace pozorovanie', 10, 'Matej Dzurov','459638524');
insert into liecba
VALUES('0931243494', 'Nevidi na lave oko', 20161212, 'Operacie', 250, 'Matej Dzurov','465379857');

insert into davkovanie
values('Paralen','pes domáci', '2 tablety denne', 'po dobu dvoch tyzdnov');
insert into davkovanie
values('Paralen','mačka domáca', '1 tableta denne', 'po dobu troch tyzdnov');
insert into davkovanie
values('Paralen','kôň domáci', '6 tabliet denne', 'po dobu štyroch tyzdnov');
insert into davkovanie
values('Alavis5','ovca domáca', '1 tableta denne', 'po dobu dvoch tyzdnov');
insert into davkovanie
values('Alavis5','pes domáci', '3 tablety denne', 'po dobu piatich tyzdnov');
insert into davkovanie
values('Alavis5','morča domáce', '1 tableta denne', 'po dobu troch dní');
insert into davkovanie
values('Paralen','užovka červená', '1 tableta denne', 'po dobu jedného tyzdna');
insert into davkovanie
values('Celadrin','kôň domáci', '8 tabliet denne', 'po dobu dvoch mesiacov');
insert into davkovanie
values('SUPREME SCIENCE Selective','morča domáce', '1 tableta denne', 'po dobu troch mesiacov');

insert into vyuziva
values('0000000001','Paralen', '2 tyzdne', '2 tablety denne');
insert into vyuziva
values('4569823659','Alavis5', '4 mesiace', '3 tablety denne');
insert into vyuziva
values('4123569871','Alavis5', '3 mesiace', '3 tablety denne');
insert into vyuziva
values('1368958258','Paralen', '3 tyzdne', '3 tablety denne');
insert into vyuziva
values('2828722872','Alavis5', '1 mesiac', '1 tableta denne');
insert into vyuziva
values('2782080287','Paralen', '4 tyzdne', '1 tableta denne');
insert into vyuziva
values('2082782872','Celadrin', '6 mesiacov', '4 tablety denne');
insert into vyuziva
values('2082782872','Alavis5', '3 mesiacov', '3 tablety denne');
insert into vyuziva
values('2872872870','SUPREME SCIENCE Selective','2 mesiace', '2 tablety denne');
insert into vyuziva
values('0931243494','Paralen','1 tyzden', '2 tablety denne');
insert into vyuziva
values('0931243494','Celadrin','1 tyzden', '2 tablety denne');
insert into vyuziva
values('0931243494','Alavis5','1 tyzden', '2 tablety denne');
insert into vyuziva
values('0931243494','SUPREME SCIENCE Selective','1 tyzden', '2 tablety denne');

insert into zaobstara
values('0000000001',8405061236);
insert into zaobstara
values('1256984036',9411190162);
insert into zaobstara
values('4569823659',9608065049);
insert into zaobstara
values('4123569871',9358178984);
insert into zaobstara
values('1368958258',8405061236);
insert into zaobstara
values('2828722872',9608065049);
insert into zaobstara
values('2782080287',9411190162);
insert into zaobstara
values('2082782872',9358178984);
insert into zaobstara
values('2872872870',9411190162);

--------------------------------------------------------------------------------
----------------------------SELECT DOTAZY PROJEKT3------------------------------
--------------------------------------------------------------------------------
--2x spojenie dvoch tabuliek
--vypise meno, datum narodenia a datum poslednej prehliadky zvierata ktoreho majitel ma krstne meno Peter
SELECT Z.meno, Z.datum_narodenia, Z.datum_poslednej_prehliadky
FROM zviera Z, majitel M
WHERE Z.cislo_op = M.cislo_op and M.meno='Peter';

--vypise cislo, diagnozu a cenu vsetkych liecob pre zvierata druhu ovca
SELECT L.cislo_liecby, L.diagnoza, L.cena
FROM liecba L, zviera Z
WHERE L.idef_cislo = Z.idef_cislo and Z.meno_druhu='ovca domáca';

--3x spojenie tabuliek
--Vypise detail liecob zvierat ktorych majitelia byvaju v Bratislave
SELECT L.cislo_liecby, L.diagnoza, L.datum_zahajenie_liecby, L.cena
FROM liecba L, zviera Z, majitel M
WHERE L.idef_cislo = Z.idef_cislo and Z.cislo_op = M.cislo_op and M.mesto<>'Bratislava';

--2x group by a agregacna funkcia
--vypise pocet liekov vyuzitych pri kazdej liecbe
SELECT L.cislo_liecby, count(V.Nazov)
FROM liecba L, vyuziva V
WHERE L.cislo_liecby = V.cislo_liecby
GROUP BY L.cislo_liecby;

--pre kazdy zivocisny druh napise datum narodenia najstarsieho clena
SELECT Z.meno_druhu, max(Z.datum_narodenia)
FROM zviera Z
GROUP BY Z.meno_druhu;

--predikat exists
--vypise meno a adresu majitelov, ktori vlastnia aspon jedneho psa
SELECT M.meno, M.priezvisko, M.mesto, M.ulica
FROM majitel M
WHERE EXISTS
(
  SELECT *
  FROM zviera Z
  WHERE Z.cislo_op = M.cislo_op and Z.meno_druhu='pes domáci');

--predikat IN
--vypise mena zamestnancov ktori nikdy nediagnostikovali zlomenu nohu
SELECT Z.meno, Z.priezvisko
FROM zamestnanec Z
WHERE Z.rodne_cislo NOT IN
(
  SELECT A.rodne_cislo
  FROM zaobstara A, liecba L
  WHERE A.cislo_liecby = L.cislo_liecby and L.diagnoza='Zlomena noha'
);

exec priemerna_cena('Zlomena noha','pes domáci');
exec vydaje_majitela('EA123456');

--------------------------------------------------------------------------------
----------------------------TRIGERRY PROJEKT4-----------------------------------
--------------------------------------------------------------------------------
-- TRIGGER KONTROLY SPRAVNOSTI FORMATU CISLA UCTU
ALTER SESSION SET nls_date_format='dd.mm.yyyy';
SET serveroutput on;
CREATE OR REPLACE TRIGGER triggerUcet
  BEFORE INSERT OR UPDATE OF cislo_uctu ON zamestnanec
  FOR EACH ROW
DECLARE
  ucet zamestnanec.cislo_uctu%TYPE;
  zakladCislaUctu varchar(10);
  modd NUMBER;
BEGIN
  ucet := :NEW.cislo_uctu;
  zakladCislaUctu := SUBSTR(ucet, 1, 10);
  modd := TO_NUMBER(SUBSTR(zakladCislaUctu, 1, 1), '9') * 6 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 2, 1), '9') * 3 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 3, 1), '9')* 7 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 4, 1), '9') * 9 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 5, 1), '9') * 10 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 6, 1), '9') * 5 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 7, 1), '9') * 8 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 8, 1), '9') * 4 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 9, 1), '9') * 2 +
          TO_NUMBER(SUBSTR(zakladCislaUctu, 10, 1), '9') * 1;
  IF (LENGTH(ucet) != 10) THEN
    RAISE_APPLICATION_ERROR (-20203, 'Ucet obsahuje malo znakov, ucet ma 10 cislic');
  END IF;
  IF MOD(modd, 11) != 0 THEN
    RAISE_APPLICATION_ERROR (-20204, 'Nespravny format cisla uctu! Pozri ISO 7046 MOD 11-2');
  END IF;
END triggerUcet;
/
show ERRORS
ALTER SESSION SET nls_date_format='dd.mm.yyyy';

-- TRIGGER PRE INKREMENTACIU IDCIEK
CREATE SEQUENCE pocetID;  -- uchova posledne ID
ALTER SESSION SET nls_date_format='dd.mm.yyyy';
CREATE OR REPLACE TRIGGER autoincrement
  BEFORE INSERT ON zviera
  FOR EACH ROW
BEGIN
  -- do IDcka ulozi hodnotu +1 sekvencie
  :new.idef_cislo  := pocetID.nextval;
END autoincrement;
/
SHOW ERRORS
ALTER SESSION SET nls_date_format='dd.mm.yyyy';

--------------------------------------------------------------------------------
-------------------------EXPLAIN PLAN A INDEX PROJEKT4--------------------------
--------------------------------------------------------------------------------
EXPLAIN PLAN FOR
SELECT meno, MAX(cena)
FROM zviera NATURAL JOIN liecba
GROUP BY cena, meno;
SELECT * FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX indexExplain ON zviera (meno);

EXPLAIN PLAN FOR
SELECT /*+ INDEX(zviera indexExplain)*/ meno, MAX(cena)
FROM zviera NATURAL JOIN liecba
GROUP BY cena, meno;
SELECT * FROM TABLE(DBMS_XPLAN.display);

--------------------------------------------------------------------------------
----------------------------UDELENIE PRAV PROJEKT4------------------------------
--------------------------------------------------------------------------------
GRANT SELECT ON liecba TO xormos00;
GRANT UPDATE ON liecba TO xormos00;
GRANT ALTER ON liecba TO xormos00;
GRANT ALL ON zviera TO xormos00;
GRANT ALL ON majitel TO xormos00;

GRANT EXECUTE ON priemerna_cena TO xormos00;
GRANT EXECUTE ON vydaje_majitela TO xormos00;

--------------------------------------------------------------------------------
-----------------------MATERIALIZOVANY POGLAD PROJEKT4--------------------------
--------------------------------------------------------------------------------
DROP MATERIALIZED VIEW pocet_jedincov;

CREATE MATERIALIZED VIEW LOG ON zviera
WITH PRIMARY KEY, ROWID(meno_druhu)
INCLUDING NEW VALUES;


CREATE MATERIALIZED VIEW pocet_jedincov
CACHE
BUILD IMMEDIATE
REFRESH FAST ON COMMIT
ENABLE QUERY REWRITE
AS 
SELECT Z.meno_druhu, count(Z.meno_druhu) as pocet_jedincov
FROM zviera Z
GROUP BY Z.meno_druhu;

GRANT ALL ON pocet_jedincov TO xormos00;

SELECT * FROM pocet_jedincov;
insert into zviera
VALUES('000000803', 'Snoop', 20040101, 20160420, 'pes domáci', 'EA977088');
COMMIT;
SELECT * FROM pocet_jedincov;

/* ENDOF script */
