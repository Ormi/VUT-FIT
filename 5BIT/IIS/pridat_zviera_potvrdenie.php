        <?  if (empty($_POST["meno"]) or ctype_lower($_POST["meno"][0]) or (!ctype_alpha($_POST["meno"]))) {
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
            ?> Zadajte celé meno majiteľa <br> <?
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
            ?> Zadajte celé meno druhu <?
          }

          if (empty($narodenie)) {
              ?> Nespravny format datumu narodenia <?
            }
          if (empty($prehliadka)) {
              ?> Nespravny format datumu poslednej prehliadky <?
            }

          if (empty($meno) or empty($majitel) or empty($druh)) {
            if (empty($majitel)) {
              ?> Majiteľ nebo nájdený <?
            }
            if (empty($meno)) {
              ?> Nesprávny formát mena <?
            }
            if (empty($druh)) {
              ?> Zivočíšny druh nebol nájdený <?
            }

          ?>
          <br>Údaje s hviezdičkou sú povinné <br>
          <form method="POST" action="?z=3&x=3">
            <table  class="table table-striped">
              <tr>
                <th class="text-danger">*Meno: </th>
                <th><input class="bg-danger" type="text" name="meno" class="login_input" value="<?php echo $meno; ?>" /></th>
              </tr>
              <tr>
                <th class="text-danger">*Živočíšny druh: </th>
                <th><input class="bg-danger" type="text" name="druh" class="login_input" value="<?php echo $druh; ?>" /></th>
              </tr>
              <tr>
                <th>Dátum narodenia: </th>
                <th><input type="text" name="narodenie" class="login_input" value="<?php echo $narodenie; ?>" /></th>
              </tr>
              <tr>
                <th>Dátum poslednej prehliadky:</th>
                <th><input type="text" name="prehliadka" class="login_input" value="<?php echo $prehliadka; ?>" /></th>
              </tr>
              <tr>
                <th class="text-danger">*Majiteľ: </th>
                <th><input class="bg-danger" type="text" name="majitel" class="login_input" value="<?php echo $menom; ?>" /> </th>
              </tr>
                </table>
                <input href="?z=3&x=3"type="submit" class="btn btn-success" value="Pridaj zviera">
          </form>
          <?

          }else {

          $query="SELECT * FROM zviera";
          $result = @mysql_query($query,$db);
          if ($result==false) {
            $cislo=1;
          } else {
            $cislo=1;
            while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
              if ($data["idef_cislo"]>$cislo) {
                $cislo=intval($data["idef_cislo"]);
              }
            }
            $cislo=$cislo+1;
          }

          $query="INSERT INTO zviera VALUES('".$cislo."', '".$meno."', '".$narodenie."', '".$prehliadka."', '".$druh."', '".$majitel."');";
          $result = @mysql_query($query,$db);
          ?><br> Polozka bola pridana <?
          }
