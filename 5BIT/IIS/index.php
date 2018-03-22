<?php

session_save_path('./tmp');

if (!isset($_SESSION))
session_start();


include "login.php";
include "portal.php";
include "db_init.php";
session_timeout();

// hlavicka
include "menu1.html";

// prve nacitanie stranky
if (!isset($_GET["menu"])){
  $_GET["menu"]=0;
}


//kdesom: 0 - hlavna stranka

//login
// 0 doktor
// 1 sestra
if(isset($_POST["name"]) and isset($_POST["pass"]) and $_POST["name"]!=""){
  $query="SELECT * FROM zamestnanec WHERE login='".$_POST["name"]."'";
  $result = @mysql_query($query,$db);
  while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
    if($data["heslo"]==$_POST["pass"]){
        switch($data["typ"]){
         case 0:
          $_GET["z"]=0;
          $_GET["x"]=0;
          $_SESSION['login']=0;
           unset($_GET["menu"]);
          $_SESSION['last_activity']=time();
          break;
          case 1:
          $_GET["z"]=0;
          $_GET["x"]=0;
          $_SESSION['login']=1;
          unset($_GET["menu"]);
          $_SESSION['last_activity']=time();
          break;

      }
    }
  }
}


// nacitanie menu uzivatela
if (!isset($_SESSION['login']) and !isset($_SESSION['log'])){
  www($_GET["menu"],$db);
} else {
  if ($_GET["z"]==20){
     unset($_GET["z"]);
     unset($_SESSION['login']);
     session_unset();
     session_destroy();
     $_GET["menu"]=5;
     ?>
       <script>
         window.location.href="index.php";
       </script>
      <?
  } else {
      zamestnanci($_GET["z"], $_GET["x"], $db, $_SESSION['login']);
  }


}
/*

*/

?>
