Údaje s hviezdičkou sú povinné <br>
      <form method="POST" action="?z=6&x=3">
        <table  class="table table-striped">
          <tr>
            <th class="text-danger">*Rodné číslo:</th>
            <th><input type="text" id="inputSuccess" name="rodne_cislo" class="login_input"/> </th>
          </tr>
          <tr>
            <th class="text-danger">*Meno: </th>
            <th><input type="text" name="meno" class="login_input"/></th>
          </tr>
          <tr>
            <th class="text-danger">*Priezvisko: </th>
            <th><input type="text" name="priezvisko" class="login_input"/></th>
          </tr>
          <tr>
            <th>Mesto: </th>
            <th><input type="text" name="mesto" class="login_input"/></th>
          </tr>
          <tr>
            <th>Ulica:</th>
            <th><input type="text" name="ulica" class="login_input"/></th>
          </tr>
          <tr>
            <th>PSČ: </th>
            <th><input type="text" name="psc" class="login_input"/> </th>
          </tr>
          <tr>
            <th>Číslo účtu: </th>
            <th><input type="text" name="cislo_uctu" class="login_input"/> </th>
          </tr>
          <tr>
            <th>Hodinová mzda: </th>
            <th><input type="text" name="hodinova_mzda" class="login_input"/> </th>
          </tr>
          <tr>
            <th class="text-danger">*Typ zamestnanca</th>
            <th>
              <label class="radio-inline"><input type="radio" name="typ" value="0">Doktor</label>
              <label class="radio-inline"><input type="radio" name="typ" value="1">Sestra</label>
            </th>
          </tr>
          <tr>
            <th>Titul: </th>
            <th><input type="text" name="titul" class="login_input" /> </th>
          </tr>
          <tr>
            <th>Špecializácia: </th>
            <th><input type="text" name="specializacia" class="login_input"/> </th>
          </tr>
          <tr>
            <th>Škola: </th>
            <th><input type="text" name="skola" class="login_input"/> </th>
          </tr>
          <tr>
            <th class="text-danger">*Login: </th>
            <th><input type="text" name="login" class="login_input"/> </th>
          </tr>
          <tr>
            <th class="text-danger">*Heslo: </th>
            <th><input type="password" name="heslo" class="login_input"/> </th>
          </tr>
            </table>
            <input href="?z=6&x=3"type="submit" class="btn btn-success" value="Pridaj Zamestnanca">
      </form>
