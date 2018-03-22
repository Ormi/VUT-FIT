
<?
header("Content-Type: text/html; charset=UTF-8");
function session_timeout() {
  if (isset($_SESSION['last_activity'])) {

    if ((time() - $_SESSION['last_activity']) > 60*60) {
      unset($_SESSION['login']);
      session_unset();
      session_destroy()
      ?>
        <script>
          window.location.href="index.php";
        </script>
      <?
    }
  }
}



// t - typ doktor(0) alebo sestra(1)
// x - case typ
// db - ukazovatel na databazu
function zamestnanci($z,$x,$db,$t){
?>

<nav class="navbar navbar-inverse navbar-fixed-top">
  <div class="container-fluid">
    <div class="navbar-header">
      <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="?z=0&x=0&y=0" >Veterinárna Klinika Mazo</a>
    </div>
    <div id="navbar" class="navbar-collapse collapse">
      <ul class="nav navbar-nav navbar-right">
        <li><a>Prihlásený:<?if ($t==1){?> Sestra <?}else{?> Doktor <?}?></a></li>
        <li><a>|  Aktuálny čas:</a></li>
        <li><a><div id="time"></div></a></li>
        <li><a>Dátum:</a></li>
        <li><a><div id="date"></div></a></li>
      </ul>
    </div>
  </div>
</nav>

<div class="container-fluid">
  <div class="row">
    <div class="col-sm-3 col-md-2 sidebar">
      <ul class="nav nav-sidebar">
        <li <?if($z==1){?>  class="selected"<?}?>><a href="?z=1&x=0&y=0">Majitelia</a></li>
        <!-- <li <?if($z==2){?>  class="selected"<?}?>><a href="?z=2&x=0&y=0" >Liecby</a></li> -->
        <li <?if($z==3){?>  class="selected"<?}?>><a href="?z=3&x=0&y=0" >Zvieratá</a></li>
        <li <?if($z==4){?>  class="selected"<?}?>><a href="?z=4&x=0&y=0" >Lieky</a></li>
        <!-- <li <?if($z==5){?>  class="selected"<?}?>><a href="?z=5" >Diagnozy</a></li> -->
        <?if($t==0){?>  <li <?if($z==0){?>  class="selected"<?}?>><a href="?z=6&x=0&y=0">Zamestnanci</a></li> <?}?>
        <li><a href="?z=20">Odhlásiť sa</a></li>
      </ul>
    </div>
    <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main">

  <?
  //TODO:
  //      branie ceskych znakov
  //      meno druhu/majitela lepsie
  //      po polozka pridana nieco dat
  //      odstranit zombie tabulky napr pri odstraneni liecby je treba odstranit aj vyuziva...
////////////////////////////////////////////////////////////////////////////////
//// Obsah Stranok
////////////////////////////////////////////////////////////////////////////////
  switch($z){
////////////////////////////////////////////////////////////////////////////////
//// 0 - UVODNA STRANKA
////////////////////////////////////////////////////////////////////////////////
  case 0:
  ?>

     <div class="center_left">
       <div class="welcome_box">
        <?if($t==0){?><p class="welcome">Vitajte v administracií. Prihlásenie prebehlo úspešne. Ste prihlasený ako doktor. <?}?>
        <?if($t==1){?><p class="welcome">Vitajte v administracií. Prihlásenie prebehlo úspešne. Ste prihlasený ako sestra. <?}?>

         </p>
       </div>
     </div>
  <?
  break;
////////////////////////////////////////////////////////////////////////////////
//// 1- SPRAVA MAJITELA
////////////////////////////////////////////////////////////////////////////////
  case 1:
  ?><table class="options">
     <tr>
     <th>
    <form method="POST" action="?z=1&x=1">
      <input type="text" name="meno_majitela" class="login_input" placeholder="Meno majitela"/>
      <input href="?z=1&x=1" class="btn btn-info" type="submit" value="Hladat majitela">
    </form>
    </th>
    <th>
    <form method="POST" action="?z=1&x=2">
      <input href="??z=1&x=2" class="btn btn-info" type="submit" value="Pridat Majitela">
    </form>
    </th>
    <th>
    <form method="POST" action="?z=1&x=7">
      <input href="??z=1&x=7" class="btn btn-info" type="submit" value="Zoznam majitelov">
    </form>
    </th>
     </tr>
    </table>
    <br>
    <!-- <button class="btn btn-info" type="submit"><a href="?z=1&x=2">Pridat Majitela</a></button> -->

     <? if ($x==1) { //Hladat majitela

            include 'hladat_majitela.php';

        }elseif ($x==2) { //pridat majitela

            include 'pridat_majitela.php';

        } elseif ($x==3) { //chyby pri pridani alebo uspesne pridanie

            include 'pridat_majitela_potvrdenie.php';

        } else if ($x==4) { //zobrazi podrobne udaje o majitelovi

            include 'zobrazit_majitela.php';

        } else if ($x==5){ //odstranenie majitela

            include 'odstranit_majitela.php';

        } else if ($x==6) { //editovanie majitela+chyby pri zlych datach editacie

          include 'editovat_majitela.php';

        } else if ($x==7) {

          include 'zoznam_majitelov.php';

        }
     ?>

  <?
  break;
  ////////////////////////////////////////////////////////////////////////////////
//// 2- SPRAVA LIECBY
////////////////////////////////////////////////////////////////////////////////
  case 2:
  ?>

  <!-- TODO -->
  Priestor pre PHP

  <?
  break;
  ////////////////////////////////////////////////////////////////////////////////
  //// 3- SPRAVA ZVIERATA
  ////////////////////////////////////////////////////////////////////////////////
  case 3:
  ?>
    <table class="options">
    <tr>
    <th>
    <form method="POST" action="?z=3&x=1">
      <input type="text" name="meno_zvierata" class="login_input" placeholder="Meno zvierata"/>
      <input href="?z=3&x=1" class="btn btn-info" type="submit" value="Hladat zviera">
    </form>
    </th>
    <th>
    <form method="POST" action="?z=3&x=2">
      <input href="?z=3&x=2" class="btn btn-info" type="submit" value="Pridat zviera">
    </form>
    </th>
    </tr>
  </table>
 <br>
    <!-- <button class="btn btn-info" type="submit"><a href="?z=1&x=2">Pridat Majitela</a></button> -->

     <? if ($x==1) { //Hladat zviera

            include 'hladat_zviera.php';

        } elseif ($x==2) { //pridat zviera

            include 'pridat_zviera.php';

        } elseif ($x==3) { //chyby pri pridani alebo uspesne pridanie

            include 'pridat_zviera_potvrdenie.php';

        } else if ($x==4) { //zobrazi podrobne udaje o zvierati

          include 'zobrazit_zviera.php';

        } else if ($x==5){ //odstranenie zvierata

          include 'odstranit_zviera.php';

        } else if ($x==6) { //editovanie zvierata+chyby pri zlych datach editacie

          include 'editovat_zviera.php';

        } else if ($x==7) { //zobraz info o liecbe

          include 'zobrazit_liecbu.php';

        }else if ($x==8) { //odstran liecbu zo systemu

        include 'odstranit_liecbu.php';

        } else if ($x==9) { //pridat liecbu

          include 'pridat_liecbu.php';

        }else if ($x==10) { //chyba pri pridani/uspesne pridanie liecby

          include 'pridat_liecbu_potvrdenie.php';

        }else if ($x==11) { //editovat liecbu

          include 'editovat_liecbu.php';

        } else if ($x==12) {//zobrazit davkovanie

          include 'zobrazit_davkovanie.php';

        } else if ($x==13) { //pridat davkovanie lieku

          include 'pridat_davkovanie.php';

        } else if ($x==14) { //chyba pri pridani/uspesne pridanie davkovania

          include 'pridat_davkovanie_potvrdenie.php';

        } else if ($x==15) { //odstranit davkovanie

          include 'odstranit_davkovanie.php';

        } else if ($x==16) { // editovat davkovanie

          include 'editovat_davkovanie.php';

        }
        ?>
  <?
  break;
  ////////////////////////////////////////////////////////////////////////////////
  //// 4- SPRAVA LIEKU
  ////////////////////////////////////////////////////////////////////////////////
  case 4:
  ?>
  <table  class="options">
  <tr>
  <th>
  <form method="POST" action="?z=4&x=1">
      <input type="text" name="meno_lieku" class="login_input" placeholder="Nazov lieku"/>
      <input href="?z=4&x=1" class="btn btn-info" type="submit" value="Hladat liek">
    </form>
    </th>
    <th>
    <form method="POST" action="?z=4&x=2">
      <input href="?z=4&x=2" class="btn btn-info" type="submit" value="Pridat liek">
    </form>
    </th>
    <th>
    <form method="POST" action="?z=4&x=7">
      <input class="btn btn-info" type="submit" href="?z=4&x=7" value="Zoznam liekov">
    </form>
    </th>
    </tr>
    </table>
    <br>
    <!-- <button class="btn btn-info" type="submit"><a href="?z=1&x=2">Pridat Majitela</a></button> -->

     <? if ($x==1) { //Hladat liek

        include 'hladat_liek.php';

        } elseif ($x==2) { //pridat liek

          include 'pridat_liek.php';

        } elseif ($x==3) { //chyby pri pridani alebo uspesne pridanie

          include 'pridat_liek_potvrdenie.php';

        } else if ($x==4) { //zobrazi podrobne udaje o lieku

          include 'zobrazit_liek.php';

        } else if ($x==5){ //odstranenie lieku

          include 'odstranit_liek.php';

        } else if ($x==6) { //editovanie lieku+chyby pri zlych datach editacie

          include 'editovat_liek.php';

        } else if ($x==7) { //zoznam liekov

          include 'zoznam_liekov.php';

        } else if ($x==8) { //vypis davkovania pre zviera

          include 'zobrazit_davkovanie_zviera.php';

        } else if ($x==9) { //pridanie davkovania lieku pre druh

          include 'pridat_davkovanie_zviera.php';

        } else if ($x==10) { //pridanie davkovania

          include 'pridat_davkovanie_zviera_potvrdenie.php';

        } else if ($x==11) { //editovanie davkovania

          include 'editovat_davkovanie_zviera.php';

        } else if ($x==12) { //odstranit davkovanie

          include 'odstranit_davkovanie_zviera.php';

        }
        ?>
  <?
  break;
  case 5:
  ?>

  <!-- TODO -->
  Priestor pre PHP

  <?
  break;
  //////////////////////////////////////////////////////////////////////////////
  //// 6 = SPRAVA ZAMESTNANCA (viditelna len pre doktora)
  //////////////////////////////////////////////////////////////////////////////

  //TODO lekarovi len niektore polozky a sestre ine
  case 6:
  ?>
  <table class="options"
  <tr>
  <th>
  <form method="POST" action="?z=6&x=1">
    <input type="text" name="meno_zamestnanca" class="login_input" placeholder="Meno zamestnanca"/>
    <input href="?z=6&x=1" class="btn btn-info" type="submit" value="Hladat zamestnanca">
  </form>
  </th>
  <th>
  <form method="POST" action="?z=6&x=2">
    <input class="btn btn-info" type="submit" href="?z=6&x=2" value="Pridaj zamestnanca">
  </form>
  </th>
  <th>
  <form method="POST" action="?z=6&x=7">
    <input class="btn btn-info" type="submit" href="?z=6&x=7" value="Zoznam zamestnancov">
  </form>
  </th>
  </tr>
  </table>
  <br>
  <? if ($x==1) { //Hladat majitela

    include 'hladat_zamestnanca.php';

    } elseif ($x==2) { //pridat zamestnanca

      include 'pridat_zamestnanca.php';

    } elseif ($x==3) { //chyby pri pridani alebo uspesne pridanie

      include 'pridat_zamestnanca_potvrdenie.php';

    } else if ($x==4) { //zobrazi podrobne udaje o zamestnancovi

      include 'zobrazit_zamestnanca.php';

    } else if ($x==5){ //odstranit zamestnanca

      include 'odstranit_zamestnanca.php';

    } else if ($x==6) { //editovat zamestnanca

      include 'editovat_zamestnanca.php';

    } else if ($x==7) { //zoznam zamestnancov

      include 'zoznam_zamestnancov.php';

    }
}
?>

    </div>
  </div>
</div>

<?
}
?>
