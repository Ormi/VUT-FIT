
          <table class="options">
          <tr>
          <th>
          <form method="POST" action="?z=3&x=8&c=<?echo $_GET["c"];?>">
            <input class="btn btn-danger" type="submit" value="Odstran zo systemu" href="?z=3&x=8&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=3&x=11&a=0&c=<?echo $_GET["c"];?>">
            <input class="btn btn-warning" type="submit" value="Editovat" href="?z=3&x=11&a=0&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=3&x=13&a=0&c=<?echo $_GET["c"];?>">
            <input class="btn btn-primary" type="submit" value="Pridat davkovanie lieku" href="?z=3&x=13&a=0&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          </tr>
          </table>
            <br>
          <?
          $query="SELECT * FROM liecba WHERE cislo_liecby='".$_GET["c"]."'";

          $result = @mysql_query($query,$db);
          $data = mysql_fetch_array($result, MYSQL_ASSOC);

          ?>
          <div class="table-responsive">
            <table class="table table-striped">
              <tbody>
                <tr>
                  <th>Meno zvieraťa:</th>
                  <th><?
                    $query="SELECT * FROM zviera WHERE idef_cislo='".$data["idef_cislo"]."'";
                    $zver = @mysql_query($query,$db);
                    $datazviera = mysql_fetch_array($zver, MYSQL_ASSOC);
                     ?><a href="?z=3&x=4&c=<?echo $datazviera['idef_cislo'];?>"> <?
                    echo $datazviera["meno"];
                   ?></a></th>
                </tr>
                <tr>
                  <th>Diagnóza:</th>
                  <th><? echo $data["diagnoza"];  ?></th>
                </tr>
                <tr>
                  <th>Dátum zahájenia liečby(formát YYYYMMDD):</th>
                  <th><? echo $data["datum_zahajenie_liecby"];  ?></th>
                </tr>
                <tr>
                  <th>Stav:</th>
                  <th><? echo $data["stav"];  ?></th>
                </tr>
                <tr>
                  <th>Cena:</th>
                  <th><? echo $data["cena"];  ?></th>
                </tr>
                <tr>
                  <th>Zodpovedajuúci lekár:</th>
                  <th><?
                    $query="SELECT * FROM zamestnanec WHERE rodne_cislo='".$data["rodne_cislo"]."'";
                    $zver = @mysql_query($query,$db);
                    $datazamestnanec = mysql_fetch_array($zver, MYSQL_ASSOC);
                    ?><a href="?z=6&x=4&c=<?echo $data['rodne_cislo'];?>"> <?
                    echo $datazamestnanec["meno"];
                    echo " ";
                    echo $datazamestnanec["priezvisko"];
                   ?></a></th>
                </tr>
              </tbody>
            </table>
            Zoznam liekov: <br>
                <?
                $query="SELECT * FROM vyuziva WHERE cislo_liecby='".$_GET["c"]."'";
                $liek = @mysql_query($query,$db);
                 ?> <table class="table table-striped"> <?
                while ($dataliek = mysql_fetch_array($liek, MYSQL_ASSOC)) {
                  ?>
                  <tr>
                    <th><a href="?z=3&x=12&c=<?echo $dataliek['nazov'];?>&d=<?echo $dataliek['cislo_liecby'];?>""><?echo $dataliek["nazov"]?></a></th>
                  </tr>
                  <?//na toto zviera sa potom bude dat kliknut
                }
                ?>
            </table>
          </div>
          <?
