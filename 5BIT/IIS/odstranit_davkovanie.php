 <?$query="DELETE FROM vyuziva WHERE cislo_liecby='".$_GET["d"]."' AND nazov='".$_GET["c"]."'";
          $result = @mysql_query($query,$db);
          ?>
          Záznam o liečbe bol úspešne odstránený.
          <?
