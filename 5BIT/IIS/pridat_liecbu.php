
          Údaje s hviezdičkou sú povinné <br>
          <form method="POST" action="?z=3&x=10&c=<?echo $_GET["c"];?>">
            <table  class="table table-striped">
              <tr>
                <th>*Meno zodpovedného lekára: </th>
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
                <th><input type="text" name="diagnoza" class="login_input"/>
                </th>
              </tr>
              <tr>
                <th>Dátum zahájenia liečby(format YYYYMMDD): </th>
                <th><input type="text" name="zahajenie" class="login_input"/></th>
              </tr>
              <tr>
                <th>Stav:</th>
                <th><input type="text" name="stav" class="login_input"/></th>
              </tr>
              <tr>
                <th>Cena: </th>
                <th><input type="text" name="cena" class="login_input"/> </th>
              </tr>
                </table>
                <input href="?z=3&x=10&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-info" value="Pridaj liecbu">
          </form>
        <?
