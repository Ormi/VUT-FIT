 <? $liek=$_POST["liek"];
          $doba_davkovania=$_POST["doba_davkovania"];
          $podavanie=$_POST["podavanie"];

          $query="INSERT INTO vyuziva VALUES('".$_GET["c"]."', '".$liek."', '".$doba_davkovania."', '".$podavanie."');";
          $result = @mysql_query($query,$db);
          ?><br> Položka bola pridaná <?
