        <?  $b=0;
          $menom="";
          if ($_GET["a"]==1) {

           if (empty($_POST["meno"]) or ctype_lower($_POST["meno"][0]) or (!ctype_alpha($_POST["meno"]))) {
            $meno="";
          } else {$meno=$_POST["meno"];}
          if (strlen($_POST["narodenie"])!=8 or (!is_numeric($_POST["narodenie"]))) {
            $narodenie="";
          } else {$narodenie=$_POST["narodenie"];}
          if (strlen($_POST["prehliadka"])!=8 or (!is_numeric($_POST["prehliadka"]))) {
            $prehliadka="";
          } else {$prehliadka=$_POST["prehliadka"];}

          $casti = explode(" ", $_POST["majitel"]);
          if (count($casti)>=2) {
            $query="SELECT * FROM majitel WHERE meno='".$casti[0]."'"."AND priezvisko='".$casti[1]."'";
            $result = @mysql_query($query,$db);
            if ($result==false) {
              $majitel="";
              $menom="";
            } else {
              $data = mysql_fetch_array($result, MYSQL_ASSOC);
              $majitel=$data["cislo_op"];
              $menom=$data["meno"]." ".$data["priezvisko"];
            }
          } else {
            $majitel="";
            $menom="";
            ?> Zadajte cele meno majitela <br> <?
          }
          

          $casti = explode(" ", $_POST["druh"]);
          if (count($casti)>=2) {
            $query="SELECT * FROM zivocisny_druh WHERE meno_druhu='".$_POST["druh"]."'";
            $result = @mysql_query($query,$db);
            if ($result==false) {
              $druh="";
            } else {
              $data = mysql_fetch_array($result, MYSQL_ASSOC);
              $druh=$data["meno_druhu"];
            }
          } else {
            $druh="";
            ?> Zadajte cele meno druhu <?
          }       

          if (empty($narodenie)) {
              ?> Nespravny format datumu narodenia <?
            }
          if (empty($prehliadka)) {
              ?> Nespravny format datumu poslednej prehliadky <?
            }

          if (empty($meno) or empty($majitel) or empty($druh)) {
            $b=1;
            if (empty($majitel)) {
              ?> Majitel nebo najdeny <?
            }
            if (empty($meno)) {
              ?> Nespravny format mena<?
            }
            if (empty($druh)) {
              ?> Zivocisny druh nebol najdeny <?
            }
          } else {
            $query="UPDATE zviera SET  meno='".$meno."', datum_narodenia='".$narodenie."', datum_poslednej_prehliadky='".$prehliadka."', meno_druhu='".$druh."', cislo_op='".$majitel."' WHERE idef_cislo='".$_GET["c"]."'";
            $result = @mysql_query($query,$db);
            ?><br> Polozka bola zmenena <?
            include 'zobrazit_zviera.php';
          }
          }
          if ($_GET["a"]==0 || $b==1) {
          $query="SELECT * FROM zviera WHERE idef_cislo='".$_GET["c"]."'";
          $zviera=@mysql_query($query,$db);
          $datazviera = mysql_fetch_array($zviera, MYSQL_ASSOC);

          $query="SELECT * FROM majitel WHERE cislo_op='".$datazviera["cislo_op"]."'";
          $majitel=@mysql_query($query,$db);
          $datamajitel = mysql_fetch_array($majitel, MYSQL_ASSOC);

            $menom=$datamajitel["meno"]." ".$datamajitel["priezvisko"];

          ?>
          <br>Udaje s hviezdickou su povinne <br>
          <form method="POST" action="?z=3&x=6&a=1&c=<?echo $_GET["c"];?>">
           <table  class="table table-striped">
              <tr>
                <th>*Meno: </th>
                <th><input type="text" name="meno" class="login_input" value="<?php echo $datazviera["meno"]; ?>" /></th>
              </tr>
              <tr>
                <th>*Zivocisny druh: </th>
                <th><select name="druh">
                    <?
                    $query="SELECT * FROM zivocisny_druh";
            
                    $result = @mysql_query($query,$db);
                    while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
                      ?>
                      <option value="<?echo $data["meno_druhu"]; ?>"<?
                      if ($data["meno_druhu"]===$datazviera["meno_druhu"]) {?> selected="selected"<?}?>><?echo $data["meno_druhu"];
                      ?></option>
                      <?
                      }
                      ?>
                      </select></th>
              </tr>
              <tr>
                <th>Datum narodenia: </th>
                <th><input type="text" name="narodenie" class="login_input" value="<?php echo $datazviera["datum_narodenia"]; ?>" /></th>
              </tr>
              <tr>
                <th>Datum poslednej prehliadky:</th>
                <th><input type="text" name="prehliadka" class="login_input" value="<?php echo $datazviera["datum_poslednej_prehliadky"]; ?>" /></th>
              </tr>
              <tr>
                <th>Majitel: </th>
                <th><input type="text" name="majitel" class="login_input" value="<?php echo $menom; ?>" /> </th>
              </tr>
                </table>
            <input href="?z=3&x=6&a=1&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-info" value="Zmenit Udaje">
          </form>
          <?
          }