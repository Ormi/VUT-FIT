<br><?
       if(isset($_POST["meno_zamestnanca"]) and $_POST["meno_zamestnanca"]!=""){
         $casti = explode(" ", $_POST["meno_zamestnanca"]);
         if (count($casti)>=2) {
           $query="SELECT * FROM zamestnanec WHERE meno='".$casti[0]."'"."AND priezvisko='".$casti[1]."'";
         } else {
           $query="SELECT * FROM zamestnanec WHERE meno='".$_POST["meno_zamestnanca"]."'"."OR priezvisko='".$_POST["meno_zamestnanca"]."'";
         }
         $result = @mysql_query($query,$db);
         ?> <table  class="table table-striped"> <?
         while ($data = mysql_fetch_array($result, MYSQL_ASSOC)) {
           ?><tr><td>
             <a href="?z=6&x=4&c=<?echo $data['rodne_cislo'];?>">
             <?echo $data['meno']; echo " "; echo $data['priezvisko']; ?></a></td></tr>
           <?
         }
         ?></table><?
       }