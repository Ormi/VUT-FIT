        <?if ($_GET["a"]==1) {

            $liek=$_POST["liek"];
            $doba_davkovania=$_POST["doba_davkovania"];
            $podavanie=$_POST["podavanie"];

            $query="UPDATE vyuziva SET nazov='".$liek."', doba_davkovaia='".$doba_davkovania."', podavanie='".$podavanie."' WHERE nazov='".$_GET["c"]."' AND cislo_liecby='".$_GET["d"]."'";
            $result = @mysql_query($query,$db);
            ?><br> Položka bola zmenená <?
            include 'zobrazit_davkovanie.php';

          } else if ($_GET["a"]==0) {
          $query="SELECT * FROM vyuziva WHERE cislo_liecby='".$_GET["d"]."' AND nazov='".$_GET["c"]."'";
          $vyuziva=@mysql_query($query,$db);
          $datavyuziva = mysql_fetch_array($vyuziva, MYSQL_ASSOC);

          ?>
          Údaje s hviezdičkou sú povinné  <br>
          <form method="POST" action="?z=3&x=16&a=1&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
            <table  class="table table-striped">
              <tr>
                <th>*Liek: </th>
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
                <th>Doba dávkovania: </th>
                <th><input type="text" name="doba_davkovania" class="login_input" value="<?php echo $datavyuziva["doba_davkovaia"]; ?>" />
                </th>
              </tr>
              <tr>
                <th>Podávanie: </th>
                <th><input type="text" name="podavanie" class="login_input" value="<?php echo $datavyuziva["podavanie"]; ?>" /></th>
              </tr>
                </table>
                <input href="?z=3&x=16&a=1&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>" type="submit" class="btn btn-success" value="Pridaj podavanie">
          </form>
          <?
          }
