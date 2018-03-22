
          <table  class="options">
          <th>
          <th>
          <form method="POST" action="?z=3&x=15&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
            <input class="btn btn-info" type="submit" value="Odstran zo systemu" href="?z=3&x=15&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
          </form>
          </th>
          <th>
          <form method="POST" action="?z=3&x=16&a=0&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
            <input class="btn btn-info" type="submit" value="Editovat" href="?z=3&x=16&a=0&c=<?echo $_GET["c"];?>&d=<?echo $_GET["d"];?>">
          </form>
          </th>
          </tr>
          </table>
            <br>
          <?
          $query="SELECT * FROM vyuziva WHERE cislo_liecby='".$_GET["d"]."' AND nazov='".$_GET["c"]."';";
          $result = @mysql_query($query,$db);
          $data = mysql_fetch_array($result, MYSQL_ASSOC);

           $query="SELECT * FROM liecba WHERE cislo_liecby='".$_GET["d"]."';";
           $resultliecba = @mysql_query($query,$db);
           $dataliecba = mysql_fetch_array($resultliecba, MYSQL_ASSOC);

           $query="SELECT * FROM zviera WHERE idef_cislo='".$dataliecba["idef_cislo"]."';";
           $resultzviera = @mysql_query($query,$db);
           $datazviera = mysql_fetch_array($resultzviera, MYSQL_ASSOC);

          ?>
          <div class="table-responsive">
            <table class="table table-striped">
              <tbody>  
                <tr>
                  <th>Meno zvierata:</th>
                  <th><a href="?z=3&x=4&c=<?echo $datazviera['idef_cislo'];?>"><? echo $datazviera["meno"];  ?></a></th>
                </tr>
                <tr>
                  <th>Diagnoza:</th>
                  <th><a href="?z=3&x=7&c=<?echo $dataliecba['cislo_liecby'];?>"><? echo $dataliecba["diagnoza"];  ?></a></th>
                </tr>
                <tr>
                  <th>Liek:</th>
                  <th><? echo $data["nazov"];  ?></th>
                </tr>
                <tr>
                  <th>Doba davkovania:</th>
                  <th><? echo $data["doba_davkovaia"];  ?></th>
                </tr>
                <tr>
                  <th>Podavanie:</th>
                  <th><? echo $data["podavanie"];  ?></th>
                </tr>                           
              </tbody>
            </table>
          </div>
          <?