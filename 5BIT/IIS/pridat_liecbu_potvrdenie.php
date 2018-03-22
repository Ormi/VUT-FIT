        <? if (strlen($_POST["diagnoza"])==0) {
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
            $query="SELECT * FROM zamestnanec WHERE rodne_cislo='".$_POST["meno"]."'";
            $result = @mysql_query($query,$db);
            $data = mysql_fetch_array($result, MYSQL_ASSOC);
            if ($data==false or $result['typ']==1) {
              $meno="";
              $menom="";
            } else {
              $rc=$data["rodne_cislo"];
              $menom=$data["meno"]." ".$data["priezvisko"];
            }

          if (empty($zahajenie)) {
            ?> Nespravny format datumu zahajenia <?
          }
          if (empty($cena)) {
            ?> Nespravny format ceny  <?
          }

          if (empty($rc) or empty($diagnoza)) {
            if (empty($rc)) {
              ?> Veterinar nebol najdeny <?
            }
            if (empty($diagnoza)) {
              ?> Musite vyplnit diagnozu <?
            }

          ?>
          <br>Udaje s hviezdickou su povinne <br>
          <form method="POST" action="?z=3&x=10&c=<?echo $_GET["c"];?>">
            <table  class="table table-striped">
              <tr>
                <th>*Meno zodpovedneho lekara: </th>
                <th>
                    <select name="meno">
                    <?
                    $query="SELECT * FROM zamestnanec WHERE typ=0";

                    $result = @mysql_query($query,$db);
                    while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
                      ?>
                      <option value="<?echo $data['rodne_cislo'];?>"><?echo $data["meno"]; echo " "; echo $data["priezvisko"];?></option>
                      <?
                      }
                      ?>
                      </select>
                </th>
              </tr>
              <tr>
                <th>*Diagnóza: </th>
                <th><input type="text" name="diagnoza" class="login_input" value="<?php echo $diagnoza; ?>"/>
                </th>
              </tr>
              <tr>
                <th>Dátum zahájenia liečby(formát YYYYMMDD): </th>
                <th><input type="text" name="zahajenie" class="login_input" value="<?php echo $zahajenie; ?>"/></th>
              </tr>
              <tr>
                <th>Stav:</th>
                <th><input type="text" name="stav" class="login_input" value="<?php echo $stav; ?>" /></th>
              </tr>
              <tr>
                <th>Cena: </th>
                <th><input type="text" name="cena" class="login_input" value="<?php echo $cena; ?>" /> </th>
              </tr>
                </table>
                <input href="?z=3&x=10&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-info" value="Pridaj liecbu">
          </form>
          <?

          }else {

          $query="SELECT * FROM liecba";
          $result = @mysql_query($query,$db);
          if ($result==false) {
            $cislo=1;
          } else {
            $cislo=1;
            while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
              if ($data["cislo_liecby"]>$cislo) {
                $cislo=intval($data["cislo_liecby"]);
              }
            }
            $cislo=$cislo+1;
          }

          $query="INSERT INTO liecba VALUES('".$cislo."', '".$diagnoza."', '".$zahajenie."', '".$stav."', '".$cena."', '".$rc."', '".$_GET["c"]."');";
          $result = @mysql_query($query,$db);
          ?><br> Polozka bola pridana <?
          }
