<?if (empty($_POST["nazov"]) or ctype_lower($_POST["nazov"][0])) {
            $nazov="";
          } else {$nazov=$_POST["nazov"];}

          $query="SELECT * FROM liek WHERE nazov='".$_POST["nazov"]."'";
          $result = @mysql_query($query,$db);
          $num=mysql_num_rows($result);
          if ($num!=0) {
            $nazov="";
            ?> V databázi sa už nachádza liek s týmto menom <br> <?
          }

          $typ=$_POST["typ"];
          $latka=$_POST["latka"];
          $kontraindikacia=$_POST["kontraindikacia"];

          if (empty($nazov)) {
            if (empty($nazov)) {
              ?> Nesprávny formát názvu lieku <br> <?
            }

          ?>
          <br>Údaje s hviezdičkou sú povinné <br>
          <form method="POST" action="?z=4&x=3">
            <table  class="table table-striped">
              <tr>
                <th class="text-danger">*Názov: </th>
                <th><input class="bg-danger" type="text" name="nazov" class="login_input" value="<?php echo $nazov; ?>" /></th>
              </tr>
              <tr>
                <th>Typ lieku: </th>
                <th><input type="text" name="typ" class="login_input" value="<?php echo $typ; ?>" /></th>
              </tr>
              <tr>
                <th>Učinná látka: </th>
                <th><input type="text" name="latka" class="login_input" value="<?php echo $latka; ?>" /></th>
              </tr>
              <tr>
                <th>Kontraindikácia:</th>
                <th><input type="text" name="kontraindikacia" class="login_input" value="<?php echo $kontraindikacia; ?>" /></th>
              </tr>
                </table>
                <input href="?z=4&x=3"type="submit" class="btn btn-info" value="Pridaj liek">
          </form>
          <?

          }else {

          $query="INSERT INTO liek VALUES('".$nazov."', '".$typ."', '".$latka."', '".$kontraindikacia."');";
          $result = @mysql_query($query,$db);
          ?><br> Položka bola pridaná <?
          }
