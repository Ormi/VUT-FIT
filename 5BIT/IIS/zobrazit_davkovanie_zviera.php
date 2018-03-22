<table  class="options">
          <tr>
          <th>
          <form method="POST" action="?z=4&x=11&a=0&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
            <input class="btn btn-warning" type="submit" value="Editovať dávkovanie"
             href="?z=4&x=11&a=0&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=4&x=12&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
            <input class="btn btn-danger" type="submit" value="Vymazať dávkovanie"
             href="?z=4&x=12&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
          </form>
          </th>
          </tr>
          </table>
            <br>
          <?

          $query="SELECT * FROM davkovanie WHERE nazov='".$_GET["c"]."' AND meno_druhu='".$_GET["d"]."'";
          $davkovanie = @mysql_query($query,$db);
          $datadavkovanie = mysql_fetch_array($davkovanie, MYSQL_ASSOC);

          ?>
          <div class="table-responsive">
            <table class="table table-striped">
              <tbody>
                <tr>
                  <th>Názov lieku:</th>
                  <th><? echo $datadavkovanie["nazov"]  ?></th>
                </tr>
                <tr>
                  <th>Zivočíšny druh:</th>
                  <th><? echo $datadavkovanie["meno_druhu"]  ?></th>
                </tr>
                <tr>
                  <th>Odporúčaná dávka:</th>
                  <th><? echo $datadavkovanie["odporucana_davka"]  ?></th>
                </tr>
                <tr>
                  <th>Doba podávania:</th>
                  <th><? echo $datadavkovanie["doba_podavania"]  ?></th>
                </tr>
              </tbody>
            </table>
          </div>
          <?
