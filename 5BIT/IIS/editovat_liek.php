  <?        $b=0;
          if ($_GET["a"]==1) {

          if (empty($_POST["nazov"]) or ctype_lower($_POST["nazov"][0])) {
            $nazov="";
          } else {$nazov=$_POST["nazov"];}

          $typ=$_POST["typ"];
          $latka=$_POST["latka"];
          $kontraindikacia=$_POST["kontraindikacia"];

          if (empty($nazov)) {
            $b=1;
            if (empty($nazov)) {
              ?> Nesprávny formát názvu lieku <?
            }
          } else {
            $query="UPDATE liek SET  nazov='".$nazov."', typ_lieku='".$typ."', ucinna_latka='".$latka."', kontraindikacia='".$kontraindikacia."' WHERE nazov='".$_GET["c"]."'";
            $result = @mysql_query($query,$db);
            ?><br> Položka bola zmenená <?
            include 'zobrazit_liek.php';
          }
          }
          if ($_GET["a"]==0 || $b==1) {
          $query="SELECT * FROM liek WHERE nazov='".$_GET["c"]."'";
          $liek=@mysql_query($query,$db);
          $dataliek = mysql_fetch_array($liek, MYSQL_ASSOC);

          ?>
          <br>Údaje s hviezdičkou sú povinné <br>
          <form method="POST" action="?z=4&x=6&a=1&c=<?echo $_GET["c"];?>">
           <table  class="table table-striped">
              <tr>
                <th>*Názov: </th>
                <th><input type="text" name="nazov" class="login_input" value="<?php echo $dataliek["nazov"]; ?>" /></th>
              </tr>
              <tr>
                <th>Typ lieku: </th>
                <th><input type="text" name="typ" class="login_input" value="<?php echo $dataliek["typ_lieku"]; ?>" /></th>
              </tr>
              <tr>
                <th>Účinná látka: </th>
                <th><input type="text" name="latka" class="login_input" value="<?php echo $dataliek["ucinna_latka"]; ?>" /></th>
              </tr>
              <tr>
                <th>Kontraindikácia:</th>
                <th><input type="text" name="kontraindikacia" class="login_input" value="<?php echo $dataliek["kontraindikacia"]; ?>" /></th>
              </tr>
                </table>
            <input href="?z=4&x=6&a=1&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-success" value="Zmenit Udaje">
          </form>
          <?
          }
