<br><?
        $query="SELECT * FROM zamestnanec";
         $result = @mysql_query($query,$db);
         ?> <table  class="table table-striped"> <?
         while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
           ?><tr><td>
             <a href="?z=6&x=4&c=<?echo $data['rodne_cislo'];?>">
             <?echo $data['meno']; echo " "; echo $data['priezvisko']; ?></a></td></tr>
           <?
         }
         ?> </table> <?