 <?      if (empty($_POST["cislo_op"]) or strlen($_POST["cislo_op"])!=8 or $_POST["cislo_op"][0]!="E"
           or $_POST["cislo_op"][1]!="A" or (!is_numeric(substr($_POST["cislo_op"],2,6)))) {
            $obciansky="";
          } else {$obciansky=$_POST["cislo_op"];}
          $query="SELECT * FROM majitel WHERE cislo_op='".$_POST["cislo_op"]."'";
          $result = @mysql_query($query,$db);
          $num=mysql_num_rows($result);
          if ($num!=0) {
            $obciansky="";
            ?> Duplikatny obciansky preukaz <br> <?
          }

          if (empty($_POST["meno"]) or ctype_lower($_POST["meno"][0]) or (!ctype_alpha($_POST["meno"]))) {
            $meno="";
          } else {$meno=$_POST["meno"];}
          if (empty($_POST["priezvisko"]) or ctype_lower($_POST["priezvisko"][0]) or (!ctype_alpha($_POST["priezvisko"]))) {
            $priezvisko="";
          } else {$priezvisko=$_POST["priezvisko"];}
          if (empty($_POST["mesto"]) or ctype_lower($_POST["mesto"][0]) or (strcspn($_POST["mesto"], '0123456789') != strlen($_POST["mesto"]))) {
            $mesto="";
          } else {$mesto=$_POST["mesto"];}
          $ulica=$_POST["ulica"];
          if (strlen($_POST["psc"])!=5 or (!is_numeric($_POST["psc"]))) {
            $psc="";
          } else {$psc=$_POST["psc"];}

          if (empty($mesto)) {
            ?> Nespravny format mesta<?
          }
          if (empty($psc)) {
            ?> Nespravny format PSC <?
          }

          if (empty($obciansky) or empty($meno) or empty($priezvisko)) {
            if (empty($obciansky)) {
              ?> Nespravny format obcianskeho preukazu <?
            }
            if (empty($meno)) {
              ?> Nespravny format mena <?
            }
            if (empty($priezvisko)) {
              ?> Nespravny format priezviska <?
            }

          ?>
          <br>Udaje s hviezdickou su povinne <br>
          <form method="POST" action="?z=1&x=3">
            <table  class="table table-striped">
              <tr>
                <th class="text-danger">*Obciansky preukaz:</th>
                <th><input class="bg-danger" type="text" name="cislo_op" class="login_input" value="<?php echo $obciansky; ?>" /> </th>
              </tr>
              <tr>
                <th class="text-danger">*Meno: </th>
                <th><input class="bg-danger" type="text" name="meno" class="login_input" value="<?php echo $meno; ?>" /></th>
              </tr>
              <tr>
                <th class="text-danger">*Priezvisko: </th>
                <th><input class="bg-danger" type="text" name="priezvisko" class="login_input" value="<?php echo $priezvisko; ?>" /></th>
              </tr>
              <tr>
                <th>Mesto: </th>
                <th><input type="text" name="mesto" class="login_input" value="<?php echo $mesto; ?>" /></th>
              </tr>
              <tr>
                <th>Ulica:</th>
                <th><input type="text" name="ulica" class="login_input" value="<?php echo $ulica; ?>" /></th>
              </tr>
              <tr>
                <th>PSC: </th>
                <th><input type="text" name="psc" class="login_input" value="<?php echo $psc; ?>" /> </th>
              </tr>
                </table>
                <input href="?z=1&x=3"type="submit" class="btn btn-info" value="Pridaj Majitela">
          </form>
          <?

          }else {

          $query="INSERT INTO majitel VALUES('".$_POST["cislo_op"]."', '".$_POST["meno"]."', '".$_POST["priezvisko"]."', '".$_POST["mesto"]."', '".$_POST["ulica"]."', '".$_POST["psc"]."');";
          $result = @mysql_query($query,$db);
          ?><br> Polozka bola pridana <?
          }
