 Údaje s hviezdičkou sú povinné <br>
          <form method="POST" action="?z=3&x=3">
            <table  class="table table-striped">
              <tr>
                <th class="text-danger">*Meno: </th>
                <th><input type="text" name="meno" class="login_input"/></th>
              </tr>
              <tr>
                <th class="text-danger">*Živočíšny druh: </th>
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
                <th>Dátum narodenia(formát YYYYMMDD): </th>
                <th><input type="text" name="narodenie" class="login_input"/></th>
              </tr>
              <tr>
                <th>Dátum poslednej prehliadky(formát YYYYMMDD):</th>
                <th><input type="text" name="prehliadka" class="login_input"/></th>
              </tr>
              <tr>
                <th class="text-danger">*Majiteľ(formát 'Meno Priezvisko'): </th>
                <th><input type="text" name="majitel" class="login_input"/> </th>
              </tr>
                </table>
                <input href="?z=3&x=3"type="submit" class="btn btn-success" value="Pridaj zviera">
          </form>
