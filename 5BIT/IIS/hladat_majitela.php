        <br><?
          if(isset($_POST["meno_majitela"]) and $_POST["meno_majitela"]!=""){
            $casti = explode(" ", $_POST["meno_majitela"]);
            if (count($casti)>=2) {
              $query="SELECT * FROM majitel WHERE meno='".$casti[0]."'"."AND priezvisko='".$casti[1]."'";
            } else {
              $query="SELECT * FROM majitel WHERE meno='".$_POST["meno_majitela"]."'"."OR priezvisko='".$_POST["meno_majitela"]."'";
            }
            $result = @mysql_query($query,$db);
            ?>
            <table class="table table-striped"> <?
            while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
              ?>
                <tr>
                <td><a href="?z=1&x=4&c=<?echo $data['cislo_op'];?>">
                <?echo $data['meno']; echo " "; echo $data['priezvisko']; ?></a></td>
                </tr>
              <?
            }
            ?> </table> <?
          } else {
            ?><a>Nezadali ste meno majiteľa</a><?
          }
