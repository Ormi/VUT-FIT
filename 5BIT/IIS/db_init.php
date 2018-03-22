<?
$DB_HOST = "localhost:/var/run/mysql/mysql.sock";
$DB_USERNAME = "xormos00";
$DB_PASSWORD = "co9bojmo";
$DB_NAME = "xormos00";

$db=mysql_connect($DB_HOST,$DB_USERNAME,$DB_PASSWORD);
mysql_query("SET NAMES utf8");

if (!mysql_select_db($DB_NAME,$db)) {
	echo "Chyba pri pripojeni k databazi.";
	exit();
}
?>
