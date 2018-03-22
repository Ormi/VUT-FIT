         <? $b=0;
          if ($_GET["a"]==1) {

            if (empty($_POST["cislo_op"]) or strlen($_POST["cislo_op"])!=8 or $_POST["cislo_op"][0]!="E"
           or $_POST["cislo_op"][1]!="A" or (!is_numeric(substr($_POST["cislo_op"],2,6)))) {
            $obciansky="";
          } else {$obciansky=$_POST["cislo_op"];}

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
            ?> Nespravny format mesta <?
          }
          if (empty($psc)) {
            ?> Nespravny format PSC <?
          }

          if (empty($obciansky) or empty($meno) or empty($priezvisko)) {
            $b=1;
            if (empty($obciansky)) {
              ?> Nespravny format obcianskeho preukazu <?
            }
            if (empty($meno)) {
              ?> Nespravny format mena <?
            }
            if (empty($priezvisko)) {
              ?> Nespravny format priezviska <?
            }
          } else {
            $query="UPDATE majitel SET  cislo_op='".$obciansky."', meno='".$meno."', priezvisko='".$priezvisko."', mesto='".$mesto."', ulica='".$ulica."', psc='".$psc."' WHERE cislo_op='".$_GET["c"]."'";
            $result = @mysql_query($query,$db);
            ?><br> Polozka bola zmenena <?
            include 'zobrazit_majitela.php';
          }
          }
          if ($_GET["a"]==0 || $b==1) {
          $query="SELECT * FROM majitel WHERE cislo_op='".$_GET["c"]."'";
          $majitel=@mysql_query($query,$db);
          $datamajitel = mysql_fetch_array($majitel, MYSQL_ASSOC);

          ?>
          <br>Udaje s hviezdickou su povinne <br>
          <form method="POST" action="?z=1&x=6&a=1&c=<?echo $_GET["c"];?>">
          <table  class="table table-striped">
          <tr>
            <th>*Obciansky preukaz:</th>
            <th><input type="text" name="cislo_op" class="login_input" value="<?php echo $datamajitel["cislo_op"];?>" /> </th>
          </tr>
          <tr>
            <th>*Meno: </th>
            <th><input type="text" name="meno" class="login_input" value="<?php echo $datamajitel["meno"];?>" /></th>
          </tr>
          <tr>
            <th>*Priezvisko: </th>
            <th><input type="text" name="priezvisko" class="login_input" value="<?php echo $datamajitel["priezvisko"];?>" /></th>
          </tr>
          <tr>
            <th>Mesto: </th>
            <th><input type="text" name="mesto" class="login_input" value="<?php echo $datamajitel["mesto"];?>" /></th>
          </tr>
          <tr>
            <th>Ulica:</th>
            <th><input type="text" name="ulica" class="login_input" value="<?php echo $datamajitel["ulica"];?>" /></th>
          </tr>
          <tr>
            <th>PSC: </th>
            <th><input type="text" name="psc" class="login_input" value="<?php echo $datamajitel["psc"];?>" /> </th>
          </tr>
            </table>
            <input href="?z=1&x=6&a=1&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-success" value="Zmenit Udaje">
          </form>
          <?
          }
