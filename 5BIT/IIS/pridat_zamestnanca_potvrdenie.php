    <?if (empty($_POST["rodne_cislo"]) or strlen($_POST["rodne_cislo"])!=10 or (!is_numeric($_POST["rodne_cislo"]))) {
        $rc="";
      } else {$rc=$_POST["rodne_cislo"];}

      $query="SELECT * FROM zamestnanec WHERE rodne_cislo='".$_POST["rodne_cislo"]."'";
      $result = @mysql_query($query,$db);
      $num=mysql_num_rows($result);
      if ($num!=0) {
        $rc="";
        ?> Duplikátne rodné číslo <br> <?
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
      if (strlen($_POST["cislo_uctu"])!=10 or (!is_numeric($_POST["cislo_uctu"]))) {
        $ucet="";
      } else {$ucet=$_POST["cislo_uctu"];}
      if ((!is_numeric($_POST["hodinova_mzda"]))) {
        $mzda="";
      } else {$mzda=$_POST["hodinova_mzda"];}
      if ($_POST["typ"][0]!="0" and $_POST["typ"][0]!="1") {
        $typ="";
        $typz=5;
      } else {$typ=$_POST["typ"]; $typz=6;}
      $query="SELECT * FROM zamestnanec WHERE login='".$_POST["login"]."'";
      $result = @mysql_query($query,$db);
      $num=mysql_num_rows($result);
      if ($num>0 or strlen($_POST["login"])>15 or empty($_POST["login"])) {
        $login="";
      } else {$login=$_POST["login"];}
      if (strlen($_POST["heslo"])>15 or empty($_POST["heslo"])) {
        $heslo="";
      } else {$heslo=$_POST["heslo"];}
      $ulica=$_POST["ulica"];
      $titul=$_POST["titul"];
      $skola=$_POST["skola"];
      $specializacia=$_POST["specializacia"];

        if (empty($mesto)) {
        ?> Nespravny format mesta<?
      }
      if (empty($psc)) {
        ?> Nespravny format PSC<?
      }
      if (empty($ucet)) {
        ?> Nespravny format cisla uctu<?
      }
      if (empty($mzda)) {
        ?> Nespravny format mzdy<?
      }

      if (empty($rc) or empty($meno) or empty($priezvisko) or $typz==5 or empty($login) or empty($heslo)) {
        if (empty($rc)) {
          ?> Nesprávny formát rodného čísla<?
        }
        if (empty($meno)) {
          ?> Nesprávny formát mena <?
        }
        if (empty($priezvisko)) {
          ?> Nesprávny formát priezviska<?
        }
        if ($typz==5) {
          ?> Nesprávny formát typu zamestnanca<?
        }
        if (empty($login)) {
        ?> Duplikatny login <?
        }
        if (empty($heslo)) {
        ?> Nespravny format hesla<?
        }

      ?>
      <br>Údaje s hviezdičkou sú povinné <br>
      <form method="POST" action="?z=6&x=3">
        <table  class="table table-striped">
          <tr>
            <th class="text-danger">*Rodné číslo:</th>
            <th><input class="bg-danger" type="text" name="rodne_cislo" class="login_input" value="<?php echo $rc;?>" /> </th>
          </tr>
          <tr>
            <th class="text-danger">*Meno: </th>
            <th><input class="bg-danger" type="text" name="meno" class="login_input" value="<?php echo $meno;?>" /></th>
          </tr>
          <tr>
            <th class="text-danger">*Priezvisko: </th>
            <th><input class="bg-danger" type="text" name="priezvisko" class="login_input" value="<?php echo $priezvisko;?>" /></th>
          </tr>
          <tr>
            <th>Mesto: </th>
            <th><input type="text" name="mesto" class="login_input" value="<?php echo $mesto;?>" /></th>
          </tr>
          <tr>
            <th>Ulica:</th>
            <th><input type="text" name="ulica" class="login_input" value="<?php echo $ulica;?>" /></th>
          </tr>
          <tr>
            <th>PSČ: </th>
            <th><input type="text" name="psc" class="login_input" value="<?php echo $psc;?>" /> </th>
          </tr>
          <tr>
            <th>Číslo účtu: </th>
            <th><input type="text" name="cislo_uctu" class="login_input" value="<?php echo $ucet;?>" /> </th>
          </tr>
          <tr>
            <th>Hodinová mzda: </th>
            <th><input type="text" name="hodinova_mzda" class="login_input" value="<?php echo $mzda;?>" /> </th>
          </tr>
          <tr>
            <th class="text-danger">*Typ zamestnanca: </th>
            <th>
            <?php if ($typ == 0) { ?>
              <label class="radio-inline"><input type="radio" name="typ" value="0" checked="checked">Doktor</label>
              <label class="radio-inline"><input type="radio" name="typ" value="1">Sestra</label>
            <?php } elseif ($typ == 1) { ?>
              <label class="radio-inline"><input type="radio" name="typ" value="0">Doktor</label>
              <label class="radio-inline"><input type="radio" name="typ" value="1" checked="checked">Sestra</label>
            <? } else { ?>
              <label class="radio-inline"><input type="radio" name="typ" value="0">Doktor</label>
              <label class="radio-inline"><input type="radio" name="typ" value="1">Sestra</label>
            <? } ?>
            </th>
          </tr>
          <tr>
            <th>Titul: </th>
            <th><input type="text" name="titul" class="login_input" value="<?php echo $titul;?>" /> </th>
          </tr>
          <tr>
            <th>Špecializácia: </th>
            <th><input type="text" name="specializacia" class="login_input" value="<?php echo $specializacia;?>" /> </th>
          </tr>
          <tr>
            <th>Škola: </th>
            <th><input type="text" name="skola" class="login_input" value="<?php echo $skola;?>" /> </th>
          </tr>
          <tr>
            <th class="text-danger">*Login: </th>
            <th><input class="bg-danger" type="text" name="login" class="login_input" value="<?php echo $login;?>" /> </th>
          </tr>
          <tr>
            <th class="text-danger">*Heslo: </th>
            <th><input class="bg-danger" type="password" name="heslo" class="login_input" value="<?php echo $heslo;?>" /> </th>
          </tr>
            </table>
            <input href="?z=6&x=3"type="submit" class="btn btn-success" value="Pridaj Zamestnanca">
      </form>
      <?

      }else {

      $query="INSERT INTO zamestnanec VALUES('".$rc."', '".$meno."', '".$priezvisko."', '".$mesto."', '".$ulica."', '".$psc."', '".$ucet."', '".$mzda."', '".$typ."', '".$titul."', '".$specializacia."', '".$skola."', '".$login."', '".$heslo."');";
      $result = @mysql_query($query,$db);
      ?><br> Položka bola pridaná <?
      }
