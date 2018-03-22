Udaje s hviezdickou su povinne <br>
          <form method="POST" action="?z=3&x=14&c=<?echo $_GET["c"];?>">
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
                      <option value="<?echo $data["nazov"];?>"><?echo $data["nazov"];?></option>
                      <?
                      }
                      ?>
                      </select>
                </th>
              </tr>
              <tr>
                <th>Doba davkovania: </th>
                <th><input type="text" name="doba_davkovania" class="login_input"/>
                </th>
              </tr>
              <tr>
                <th>Podavanie: </th>
                <th><input type="text" name="podavanie" class="login_input"/></th>
              </tr>
                </table>
                <input href="?z=3&x=14&c=<?echo $_GET["c"];?>" type="submit" class="btn btn-info" value="Pridaj podavanie">
          </form>
        <?