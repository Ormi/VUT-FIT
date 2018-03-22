   <?     $query="DELETE FROM liek WHERE nazov='".$_GET["c"]."'";
          $result = @mysql_query($query,$db);
          ?>
          Záznam o lieku bol úspešne odstránený.
          <?
