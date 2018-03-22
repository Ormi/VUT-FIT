<table  class="options">
          <tr>
          <th>
          <form method="POST" action="?z=4&x=5&c=<?echo $_GET["c"];?>">
            <input class="btn btn-danger" type="submit" value="Odstráň zo systému" href="?z=3&x=5&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=4&x=6&a=0&c=<?echo $_GET["c"];?>">
            <input class="btn btn-warning" type="submit" value="Editovať" href="?z=3&x=6&a=0&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=4&x=9&a=0&c=<?echo $_GET["c"];?>">
            <input class="btn btn-primary" type="submit" value="Pridať dávkovanie pre druh" href="?z=4&x=9&a=0&c=<?echo $_GET["c"];?>">
          </form>
          </th>
          </tr>
          </table>
            <br>
          <?
          $query="SELECT * FROM liek WHERE nazov='".$_GET["c"]."'";

          $result = @mysql_query($query,$db);
          $data = mysql_fetch_array($result, MYSQL_ASSOC);

          ?>
          <div class="table-responsive">
            <table class="table table-striped">
              <tbody>
                <tr>
                  <th>Názov:</th>
                  <th><? echo $data["nazov"]  ?></th>
                </tr>
                <tr>
                  <th>Typ lieku:</th>
                  <th><? echo $data["typ_lieku"]  ?></th>
                </tr>
                <tr>
                  <th>Učinná látka:</th>
                  <th><? echo $data["ucinna_latka"]  ?></th>
                </tr>
                <tr>
                  <th>Kontraindikácia:</th>
                  <th><? echo $data["kontraindikacia"]  ?></th>
                </tr>
              </tbody>
            </table>
            Dávkovanie pre živočíšne druhy:<br>
                <?
                $query="SELECT * FROM davkovanie WHERE nazov='".$data["nazov"]."'";
                $davkovanie = @mysql_query($query,$db);
                ?> <table class="table table-striped"> <?
                while ($datadavkovanie = mysql_fetch_array($davkovanie, MYSQL_ASSOC)) {
                  ?>
                  <tr>
                    <th><a href="?z=4&x=8&c=<?echo $datadavkovanie['nazov'];?>&d=<?echo $datadavkovanie['meno_druhu'];?>">
                    <?echo $datadavkovanie["meno_druhu"]?></a></th>
                  </tr>
                  <?//na toto zviera sa potom bude dat kliknut
                }
                ?>
            </table>
          </div>
