create table zamestnanec (
  rodne_cislo varchar(10) not null,
  meno char(20) not null,
  priezvisko char(20) not null,
  mesto char(20),
  ulica char(20) not null,
  psc int,
  cislo_uctu varchar(10) not null,
  hodinova_mzda int not null,
  typ int(5) not null,
  titul char(6),
  specializacia char (20),
  skola char (20),
  login varchar(15),
  heslo varchar(15),
  check(round(rodne_cislo/11.0) = rodne_cislo/11.0),
  primary key (rodne_cislo, login),
  unique key (rodne_cislo, login)
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
  foreign key (meno_druhu) references zivocisny_druh(meno_druhu),
  foreign key (cislo_op) references majitel(cislo_op)
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
  rodne_cislo char(10) not null,
  idef_cislo int not null,
  primary key (cislo_liecby),
  foreign key (idef_cislo) references zviera(idef_cislo),
  foreign key (rodne_cislo) references zamestnanec(rodne_cislo)
);

create table davkovanie (
  nazov varchar(40) not null,
  meno_druhu varchar(40) not null,
  odporucana_davka varchar(50) not null,
  doba_podavania varchar(50),
  primary key (nazov, meno_druhu),
  foreign key (nazov) references liek(nazov) on delete cascade,
  foreign key (meno_druhu) references zivocisny_druh(meno_druhu) on delete cascade
);

create table vyuziva (
  cislo_liecby varchar(10),
  nazov varchar(40),
  doba_davkovania varchar(50),
  podavanie varchar(50),
  primary key (cislo_liecby, nazov),
  foreign key(cislo_liecby) references liecba(cislo_liecby) on delete cascade,
  foreign key(nazov) references liek(nazov) on delete cascade
);

insert into zamestnanec
VALUES('8405061236', 'Matej', 'Dzurov', 'Brno', 'Kolejni 2', 61200, '1234567890', 5, 0, 'MVDR.', 'kone', "", 'doktor1' , 'doktor'); 
insert into zamestnanec
VALUES('9411190162', 'Matej', 'Bielik', 'Bratislava', 'Alstrova 3', 83106, '9133519265', 6, 0, 'MVDR.', 'psy', '', 'doktor2' , 'doktor');
insert into zamestnanec
VALUES('8962161681', 'Jana', 'Veselá', 'Bratislava', 'Bulharská 7', 82104, '9438805999', 3, 1, "", "", 'Sv. Alžbety', 'sestra1' , 'sestra');

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
VALUES('0000000001', 'Zotavenie po ukusnuti', 20160323, 'Lieky proti bolesti', 10, '8405061236','000000004');
insert into liecba
VALUES('1256984036', 'Nevladnost, mdlobost', 20160108, 'Domace pozorovanie', 5, '8405061236','459638524');
insert into liecba
VALUES('4569823659', 'Problemy s klbom', 20160215, 'Nasadenie liekov na klby pre psi a macky', 12, '9411190162','000000001');
insert into liecba
VALUES('4123569871', 'Zlomena noha', 20160129, 'Sadra', 120, '8405061236','156389524');
insert into liecba
VALUES('1368958258', 'Rozhriznuty chvost po bitke', 20160315, 'Lieky proti bolesti', 10, '9411190162','000000002');
insert into liecba
VALUES('2828722872', 'Zlomena noha', 20160213, 'Sadra', 120, '8405061236','139043890');
insert into liecba
VALUES('2782080287', 'Rozhriznute ucho', 20160208, 'Lieky proti bolesti', 12, '9411190162','459638524');
insert into liecba
VALUES('2082782872', 'Zlomena noha', 20160115, 'Sadra zacina sa liecit', 150, '9411190162','243265789');
insert into liecba
VALUES('2872872870', 'Privelky nevysvetlitelny zapach', 20160321, 'Lieky pre morcata a zapach', 20, '8405061236','000000003');
insert into liecba
VALUES('3959430842', 'Samovolne vypadavanie vlny', 20150908, 'Domace pozorovanie', 10, '9411190162','459638524');
insert into liecba
VALUES('0931243494', 'Nevidi na lave oko', 20161212, 'Operacie', 250, '8405061236','465379857');

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

/* PROCEDURES */

commit;
