<br><?
          if(isset($_POST["meno_zvierata"]) and $_POST["meno_zvierata"]!=""){
              $query="SELECT * FROM zviera WHERE meno='".$_POST["meno_zvierata"]."' OR idef_cislo='".$_POST["meno_zvierata"]."'";

            $result = @mysql_query($query,$db);
            ?> <table class="table table-striped"> <?
            while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
              ?>
                <tr>
                <td><a href="?z=3&x=4&c=<?echo $data['idef_cislo'];?>">
                <?echo $data['meno'];?></a></td>
                </tr>
              <?
            }
            ?> </table> <?
          } else {
            ?><a>Nezadali ste meno alebo identifikačne číslo zvieraťa</a><?
          }
