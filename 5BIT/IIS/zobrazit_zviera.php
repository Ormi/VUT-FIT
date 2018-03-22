 <table class="options">
          <tr>
          <th>
          <form method="POST" action="?z=3&x=5&c=<?echo $_GET["c"];?>">
            <input class="btn btn-danger" type="submit" value="Odstráň zo systému" href="?z=3&x=5&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=3&x=6&a=0&c=<?echo $_GET["c"];?>">
            <input class="btn btn-warning" type="submit" value="Editovať" href="?z=3&x=6&a=0&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=3&x=9&a=0&c=<?echo $_GET["c"];?>">
            <input class="btn btn-primary" type="submit" value="Pridať liečbu" href="?z=3&x=9&a=0&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          </tr>
          </table>
            <br>
          <?
          $query="SELECT * FROM zviera WHERE idef_cislo='".$_GET["c"]."'";

          $result = @mysql_query($query,$db);
          $data = mysql_fetch_array($result, MYSQL_ASSOC);

          ?>
          <div class="table-responsive">
            <table class="table table-striped">
              <tbody>
                <tr>
                  <th>Meno:</th>
                  <th><? echo $data["meno"]  ?></th>
                </tr>
                <tr>
                  <th>Dátum narodenia(formát YYYYMMDD):</th>
                  <th><? echo $data["datum_narodenia"]  ?></th>
                </tr>
                <tr>
                  <th>Dátum poslednej prehliadky(formát YYYYMMDD):</th>
                  <th><? echo $data["datum_poslednej_prehliadky"]  ?></th>
                </tr>
                <tr>
                  <th>Zivočíšny druh:</th>
                  <th><? echo $data["meno_druhu"]  ?></th>
                </tr>
                <tr>
                  <th>Majiteľ:</th>
                  <th><?
                    $query="SELECT * FROM majitel WHERE cislo_op='".$data["cislo_op"]."'";
                    $zver = @mysql_query($query,$db);
                    $datamajitel = mysql_fetch_array($zver, MYSQL_ASSOC);
                    ?><a href="?z=1&x=4&c=<?echo $datamajitel['cislo_op'];?>"> <?
                    echo $datamajitel["meno"];
                    echo " ";
                    echo $datamajitel["priezvisko"];
                    ?> </a> <?
                   ?></th>
                </tr>
              </tbody>
            </table>

            Zoznam liečob: <br>
                <?
                $query="SELECT * FROM liecba WHERE idef_cislo='".$_GET["c"]."'
                ORDER BY datum_zahajenie_liecby DESC;";
                $liecba = @mysql_query($query,$db);
                 ?> <table class="table table-striped">
                 <tr>
                  <th>Diagnóza:</th>
                  <th>Dátum zahájenia liečby:</th>
                 </tr>
                  <?
                while ($dataliecba = mysql_fetch_array($liecba, MYSQL_ASSOC)) {
                  ?>
                  <tr>
                    <td><a href="?z=3&x=7&c=<?echo $dataliecba['cislo_liecby'];?>"><?echo $dataliecba["diagnoza"]?></a></td>
                    <td><a href="?z=3&x=7&c=<?echo $dataliecba['cislo_liecby'];?>"><?echo $dataliecba["datum_zahajenie_liecby"]?></a></td>
                  </tr>
                  <?//na toto zviera sa potom bude dat kliknut
                }
                ?>
            </table>
          </div>
