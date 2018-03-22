<?$query="DELETE FROM majitel WHERE cislo_op='".$_GET["c"]."'";
          $result = @mysql_query($query,$db);
          ?>
          Záznam o majiteľovi bol úspešne odstránený.
          <?
