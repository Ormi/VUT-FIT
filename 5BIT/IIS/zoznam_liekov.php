<br><?
        $query="SELECT * FROM liek";
         $result = @mysql_query($query,$db);
         ?> <table  class="table table-striped"> <?
         while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
           ?><tr><td>
             <a href="?z=4&x=4&c=<?echo $data['nazov'];?>">
             <?echo $data['nazov'];?></a></td></tr>
           <?
         }
         ?> </table> <?