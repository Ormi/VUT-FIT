      <?    $b=0;
          $menom="";
          if ($_GET["a"]==1) {

           if (strlen($_POST["diagnoza"])==0) {
            $diagnoza="";
          } else {$diagnoza=$_POST["diagnoza"];}
          if (strlen($_POST["zahajenie"])!=8 or (!is_numeric($_POST["zahajenie"]))) {
            $zahajenie="";
          } else {$zahajenie=$_POST["zahajenie"];}
          $stav=$_POST["stav"];
          if (!is_numeric($_POST["cena"])) {
            $cena="";
          } else {$cena=$_POST["cena"];}

          $casti = explode(" ", $_POST["meno"]);
          if (count($casti)>=2) {
            $query="SELECT * FROM zamestnanec WHERE meno='".$casti[0]."'"."AND priezvisko='".$casti[1]."'";
            $result = @mysql_query($query,$db);
            $data = mysql_fetch_array($result, MYSQL_ASSOC);
            if ($data==false or $result['typ']==1) {
              $meno="";
              $menom="";
            } else {
              $rc=$data["rodne_cislo"];
              $menom=$data["meno"]." ".$data["priezvisko"];
            }
          } else {
            $rc="";
            $menom="";
            ?> Zadajte cele meno zamestnanca <?
          }

          if (empty($zahajenie)) {
            ?> Nespravny format datumu zahajenia <?
          }
          if (empty($cena)) {
            ?> Nespravny format ceny<?
          }

          if (empty($rc) or empty($diagnoza)) {
            $b=1;
            if (empty($rc)) {
              ?> Veterinar nebol najdeny <?
            }
            if (empty($diagnoza)) {
              ?> Musite vyplnit diagnozu <?
            }

          } else {
            $query="UPDATE liecba SET diagnoza='".$diagnoza."', datum_zahajenie_liecby='".$zahajenie."', stav='".$stav."', cena='".$cena."', rodne_cislo='".$rc."' WHERE cislo_liecby='".$_GET["c"]."'";
            $result = @mysql_query($query,$db);
            ?><br> Polozka bola zmenena <?
            include 'zobrazit_liecbu.php';
          }
          }
          if ($_GET["a"]==0 || $b==1) {
          $query="SELECT * FROM liecba WHERE cislo_liecby='".$_GET["c"]."'";
          $liecba=@mysql_query($query,$db);
          $dataliecba = mysql_fetch_array($liecba, MYSQL_ASSOC);

          $query="SELECT * FROM zamestnanec WHERE rodne_cislo='".$dataliecba["rodne_cislo"]."'";
          $zamestnanec=@mysql_query($query,$db);
          $datazamestnanec = mysql_fetch_array($zamestnanec, MYSQL_ASSOC);

          $menom=$datazamestnanec["meno"]." ".$datazamestnanec["priezvisko"];

          ?>
          <br>Udaje s hviezdickou su povinne <br>
          <form method="POST" action="?z=3&x=11&a=1&c=<?echo $_GET["c"];?>">
            <table  class="table table-striped">
              <tr>
                <th>*Meno zodpovedneho lekara: </th>
                <th>
                    <select name="meno">
                    <?
                    $query="SELECT * FROM zamestnanec WHERE typ=0";

                    $result = @mysql_query($query,$db);
                    while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {

                      ?><option value="<?echo $data["meno"]; echo " "; echo $data["priezvisko"];?>"<?
                      if ($data["rodne_cislo"]===$datazamestnanec["rodne_cislo"]) {?> selected="selected"<?}?>><?echo $data["meno"]; echo " "; echo $data["priezvisko"];?>
                      <?
                      }
                      ?>
                      </select>
                </th>
              </tr>
              <tr>
                <th>*Diagnoza: </th>
                <th><input type="text" name="diagnoza" class="login_input" value="<?php echo $dataliecba["diagnoza"]; ?>" />
                </th>
              </tr>
              <tr>
                <th>Datum zahajenia liecby(formát YYYYMMDD): </th>
                <th><input type="text" name="zahajenie" class="login_input" value="<?php echo $dataliecba["datum_zahajenie_liecby"]; ?>" /></th>
              </tr>
              <tr>
                <th>Stav:</th>
                <th><input type="text" name="stav" class="login_input" value="<?php echo $dataliecba["stav"]; ?>" /></th>
              </tr>
              <tr>
                <th>Cena: </th>
                <th><input type="text" name="cena" class="login_input" value="<?php echo $dataliecba["cena"]; ?>" /> </th>
              </tr>
                </table>
                <input href="?z=3&x=11&a=1&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-info" value="Edituj liecbu">
          </form>
          <?
          }
