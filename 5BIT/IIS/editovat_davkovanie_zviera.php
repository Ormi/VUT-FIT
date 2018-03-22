     <?     $b=0;
          if ($_GET["a"]==1) {

          if (empty($_POST["liek"])) {
            $nazov="";
          } else {$nazov=$_POST["liek"];}

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

          $davka=$_POST["davka"];
          $doba=$_POST["doba"];

          if (empty($nazov) or empty($druh)) {
            $b=1;
            if (empty($nazov)) {
              ?> Nesprávny formát názvu lieku <?
            }
            if (empty($druh)) {
              ?> Nesprávny formát živočíšneho druhu <?
            }

          } else {
            $query="UPDATE davkovanie SET  nazov='".$nazov."', meno_druhu='".$druh."', odporucana_davka='".$davka."', doba_podavania='".$doba."' WHERE nazov='".$_GET["c"]."' AND meno_druhu='".$_GET["d"]."'";
            $result = @mysql_query($query,$db);
            ?><br> Položka bola zmenená <?
            include 'zobrazit_davkovanie_zviera.php';
          }
          }
          if ($_GET["a"]==0 || $b==1) {
          $query="SELECT * FROM davkovanie WHERE nazov='".$_GET["c"]."' AND meno_druhu='".$_GET["d"]."'";
          $davkovanie=@mysql_query($query,$db);
          $datadavkovanie = mysql_fetch_array($davkovanie, MYSQL_ASSOC);

          ?>
          <br>Údaje s hviezdičkou sú povinné <br>
          <form method="POST" action="?z=4&x=11&a=1&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
           <table  class="table table-striped">
                <tr>
                <th>*Názov: </th>
                  <th>
                    <select name="liek">
                    <?
                    $query="SELECT * FROM liek";

                    $result = @mysql_query($query,$db);
                    while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
                      ?>
                      <option value="<?echo $data["nazov"]; ?>"<?
                      if ($data["nazov"]===$datadavkovanie["nazov"]) {?> selected="selected"<?}?>><?echo $data["nazov"];
                      ?></option>
                      <?
                      }
                      ?>
                      </select>
                </th>
              </tr>
              <tr>
                <th>*Živočíšny druh: </th>
                <th><select name="druh">
                    <?
                    $query="SELECT * FROM zivocisny_druh";

                    $result = @mysql_query($query,$db);
                    while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
                      ?>
                      <option value="<?echo $data["meno_druhu"]; ?>"<?
                      if ($data["meno_druhu"]===$datadavkovanie["meno_druhu"]) {?> selected="selected"<?}?>><?echo $data["meno_druhu"];
                      ?></option>
                      <?
                      }
                      ?>
                      </select></th>
              </tr>
              </tr>
              <tr>
                <th>Odporúčaná dávka: </th>
                <th><input type="text" name="davka" class="login_input" value="<?php echo $datadavkovanie["odporucana_davka"]; ?>" /></th>
              </tr>
              <tr>
                <th>Doba podávania:</th>
                <th><input type="text" name="doba" class="login_input" value="<?php echo $datadavkovanie["doba_podavania"]; ?>" /></th>
              </tr>
                </table>
            <input href="?z=4&x=11&a=1&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>" type="submit" class="btn btn-success" value="Zmenit Udaje">
          </form>
          <?
          }
