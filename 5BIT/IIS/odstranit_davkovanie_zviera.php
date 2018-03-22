  <?        $query="DELETE FROM davkovanie WHERE nazov='".$_GET["c"]."' AND meno_druhu='".$_GET["d"]."'";
          $result = @mysql_query($query,$db);
          ?>
          Záznam o dávkovaní pre druh bol úspešne odstránený.
          <?
