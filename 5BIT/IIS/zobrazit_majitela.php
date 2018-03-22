<table class="options">
          <tr>
          <th>
          <form method="POST" action="?z=1&x=5&c=<?echo $_GET["c"];?>">
            <input class="btn btn-danger" type="submit" value="Odstran zo systemu" href="?z=1&x=5&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=1&x=6&a=0&c=<?echo $_GET["c"];?>">
            <input class="btn btn-warning" type="submit" value="Editovat" href="?z=1&x=6&a=0&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          </tr>
          </table>
            <br>
          <?
          $query="SELECT * FROM majitel WHERE cislo_op='".$_GET["c"]."'";

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
              </tbody>
            </table>

            Zoznam zvierat: <br>
                <?
                $query="SELECT * FROM zviera WHERE cislo_op='".$data["cislo_op"]."'";
                $zver = @mysql_query($query,$db);
                 ?> <table class="table table-striped"> <?
                while ($datazver = mysql_fetch_array($zver, MYSQL_ASSOC)) {
                  ?>
                  <tr>
                    <th><a href="?z=3&x=4&c=<?echo $datazver['idef_cislo'];?>"><?echo $datazver["meno"]?></a></th>
                  </tr>
                  <?//na toto zviera sa potom bude dat kliknut
                }
                ?>
            </table>
          </div>
