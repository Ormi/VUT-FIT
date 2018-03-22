        <?
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
            ?> Zadajte celé meno druhu <br> <?
          }

          $davka=$_POST["davka"];
          $doba=$_POST["doba"];

          if (empty($nazov) or empty($druh)) {
            if (empty($nazov)) {
              ?> Nesprávny formát názvu lieku <br> <?
            }
            if (empty($druh)) {
              ?> Nesprávny formát živočíšneho druhu <br> <?
            }

          ?>
          <br>Údaje s hviezdičkou sú povinné <br>
          <form method="POST" action="?z=4&x=10&c=<?echo $_GET["c"];?>">
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
                      if ($data["nazov"]===$_GET["c"]) {?> selected="selected"<?}?>><?echo $data["nazov"];
                      ?></option>
                      <?
                      }
                      ?>
                      </select>
                </th>
              </tr>
              <tr>
                <th>*Živočíšny druh: </th>
                <th>
                    <select name="druh">
                    <?
                    $query="SELECT * FROM zivocisny_druh";

                    $result = @mysql_query($query,$db);
                    while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
                      ?>
                      <option value="<?echo $data["meno_druhu"];?>"><?echo $data["meno_druhu"];?></option>
                      <?
                      }
                      ?>
                      </select>
                </th>
              </tr>
              <tr>
                <th>Odporúčaná dávka: </th>
                <th><input type="text" name="davka" class="login_input" value="<?php echo $davka; ?>" /></th>
              </tr>
              <tr>
                <th>Doba podávania:</th>
                <th><input type="text" name="doba" class="login_input" value="<?php echo $doba; ?>" /></th>
              </tr>
                </table>
                <input href="?z=4&x=10&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-success" value="Pridaj davkovanie">
          </form>
          <?

          }else {

          $query="INSERT INTO davkovanie VALUES('".$nazov."', '".$druh."', '".$davka."', '".$doba."');";
          $result = @mysql_query($query,$db);
          ?><br> Položka bola pridaná <?
          }
