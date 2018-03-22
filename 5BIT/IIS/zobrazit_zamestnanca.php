<table  class="options">
      <tr>
      <th>
        <form method="POST" action="?z=6&x=5&c=<?echo $_GET["c"];?>">
          <input class="btn btn-danger" type="submit" value="Odstran zo systemu" href="?z=6&x=5&c=<?echo $_GET["c"];?>">
        </form>
        </th>
        <th>
        <form method="POST" action="?z=6&x=6&a=0&c=<?echo $_GET["c"];?>">
          <input class="btn btn-warning" type="submit" value="Editovat" href="?z=6&x=6&a=0&c=<?echo $_GET["c"];?>">
        </form>
        </th>
        </tr>
        </table>
        <br>
      <?
      $query="SELECT * FROM zamestnanec WHERE rodne_cislo='".$_GET["c"]."'";

      $result = @mysql_query($query,$db);
      $data = mysql_fetch_array($result, MYSQL_ASSOC);

      ?>
      <div class="table-responsive">
        <table class="table table-striped">
          <tbody>
            <tr>
              <th>Rodné číslo:</th>
              <th><? echo $data["rodne_cislo"]  ?></th>
            </tr>
            <tr>
              <th>Meno:</th>
              <th><? echo $data["meno"]  ?></th>
            </tr>
            <tr>
              <th>Priezvisko:</th>
              <th><? echo $data["priezvisko"]  ?></th>
            </tr>
            <tr>
              <th>Mesto:</th>
              <th><? echo $data["mesto"]  ?></th>
            </tr>
            <tr>
              <th>Ulica:</th>
              <th><? echo $data["ulica"]  ?></th>
            </tr>
            <tr>
              <th>PSČ:</th>
              <th><? echo $data["psc"]  ?></th>
            </tr>
            <tr>
              <th>Číslo účtu:</th>
              <th><? echo $data["cislo_uctu"]  ?></th>
            </tr>
            <tr>
              <th>Hodinová mzda:</th>
              <th><? echo $data["hodinova_mzda"]  ?></th>
            </tr>
            <tr>
              <th>Typ zamestnanca:</th>
              <th>
              <? if ($data["typ"] == 0) { ?>
                  Doktor
              <? } elseif ($data["typ"] == 1) {?>
                  Sestra
                <? } ?>
              </th>
            </tr>
            <? if ($data["typ"]==0) { ?>
            <tr>
              <th>Titul:</th>
              <th><? echo $data["titul"]  ?></th>
            </tr>
            <tr>
              <th>Specializacia:</th>
              <th><? echo $data["specializacia"]  ?></th>
            </tr>
            <? } ?>
            <? if ($data["typ"]==1) { ?>
            <tr>
              <th>Skola:</th>
              <th><? echo $data["skola"]  ?></th>
            </tr>
            <? } ?>
            <tr>
              <th>Login:</th>
              <th><? echo $data["login"]  ?></th>
            </tr>
            <tr>
              <th>Heslo:</th>
              <th><? echo "*****"  ?></th>
            </tr>
          </tbody>
        </table>

          <? if ($data["typ"]==0) { ?>
          Ma na starosti liecby: <br>
                <?
                $query="SELECT * FROM liecba WHERE rodne_cislo='".$_GET["c"]."'
                ORDER BY datum_zahajenie_liecby DESC;";
                $liecba = @mysql_query($query,$db);
                 ?> <table class="table table-striped">
                 <tr>
                  <th>Diagnoza:</th>
                  <th>Zviera:</th>
                  <th>Datum zahajenia liecby:</th>
                 </tr>
                  <?
                while ($dataliecba = mysql_fetch_array($liecba, MYSQL_ASSOC)) {

                  $query="SELECT * FROM zviera WHERE idef_cislo='".$dataliecba['idef_cislo']."'";
                  $zviera = @mysql_query($query,$db);
                  $datazviera = mysql_fetch_array($zviera, MYSQL_ASSOC)

                  ?>
                  <tr>
                    <td><a href="?z=3&x=7&c=<?echo $dataliecba['cislo_liecby'];?>"><?echo $dataliecba["diagnoza"]?></a></td>
                    <td><a href="?z=3&x=4&c=<?echo $datazviera['idef_cislo'];?>"><?echo $datazviera["meno"]?></a></td>
                    <td><a href="?z=3&x=7&c=<?echo $dataliecba['cislo_liecby'];?>"><?echo $dataliecba["datum_zahajenie_liecby"]?></a></td>
                  </tr>
                  <?//na toto zviera sa potom bude dat kliknut
                }
                ?>
            </table>
          <? } ?>
      </div>
      <?
