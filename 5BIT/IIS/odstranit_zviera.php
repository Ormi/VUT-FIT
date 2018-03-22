       <?   $query="DELETE FROM zviera WHERE idef_cislo='".$_GET["c"]."'";
          $result = @mysql_query($query,$db);
          ?>
          Zaznam o zvierati bol úspešne odstránený.
          <?
