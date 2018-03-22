<br><?
          if(isset($_POST["meno_lieku"]) and $_POST["meno_lieku"]!=""){
            $query="SELECT * FROM liek WHERE nazov='".$_POST["meno_lieku"]."'";

            $result = @mysql_query($query,$db);
            ?><table class="table table-striped"> <?
            while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
              ?><tr><td>
               <a href="?z=4&x=4&c=<?echo $data['nazov'];?>">
               <?echo $data['nazov'];?></a></td></tr>
              <?
            }
            ?> </table> <?
          } else {
            ?><a>Nezadali ste názov lieku</a><?
          }
