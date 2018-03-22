Údaje s hviezdičkou sú povinné  <br>
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
                <th>*Zivočíšny druh: </th>
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
                <th><input type="text" name="davka" class="login_input"/></th>
              </tr>
              <tr>
                <th>Doba podávania:</th>
                <th><input type="text" name="doba" class="login_input"/></th>
              </tr>
                </table>
                <input href="?z=4&x=10&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-success" value="Pridaj liek">
          </form>
