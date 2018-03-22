        <?
        $query="SELECT * FROM majitel ORDER BY priezvisko, meno";
         $result = @mysql_query($query,$db);
         ?> <table  class="table table-striped"> <?
         while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
           ?><tr><td>
             <a href="?z=1&x=4&c=<?echo $data['cislo_op'];?>">
             <?echo $data['priezvisko']; echo " "; echo $data['meno']; ?></a></td></tr>
           <?
         }
         ?> </table> <?
