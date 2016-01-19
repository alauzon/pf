<?

define(IMG_YES, '<img src="http://etaware.com/gimages2/accept.png"> ');
define(IMG_NO, '<img src="http://etaware.com/gimages2/delete.png"> ');
define(CA, ", ");
// file used to see if hostvise can be installed

$numfail = 0;

function works($fn)
{
	global $numfail;
	
	if(function_exists($fn))
		return IMG_YES;
	else { $numfail++; return IMG_NO; }
}

function version($have,$need)
{
	global $numfail;

	if($have > $need)
		return IMG_YES;
	else { $numfail++; return IMG_NO; }
}

$functions = array("sizeof","mail","time","date","array_keys","fsockopen","fopen","fclose","fputs","ord","implode","explode","file_get_contents","phpversion","mysql_query","mysql_error","mysql_fetch_array","mysql_fetch_assoc","mysql_connect","mysql_get_server_info",);


print '<body style="background-color:white;">
<title>HostVise Compatiblity Check</title><br><br><br><center><div style="border:1px solid #bbb; width:500px; text-align:left; padding:25px; font-family:tahoma, Century Gothic, sans-serif; background-color:white; font-size:12px;">';

print '<img src="http://etaware.com/hostvise/wp-content/themes/eProduct/images/hostvise.png"> <br><br>
<h1><img src="http://etaware.com/gimages/box_open.png" border="0"> Checking HostVise compatiblity:</h1>
<hr>';


//php
print '<b>PHP Version: </b> ';
print version(phpversion(),5.0);
print phpversion();
print ' [needs >5.0]<br>';

//mysql
print '<b>Mysql Detected:</b> ';
print works("mysql_connect");
print ' [needs >5.0]<br>';


// functions
print '<br><br><b>Function Check: </b>';
foreach($functions as $function)
	print works($function) . strtoupper($function) . CA;

print '<i>end check.</i><br><hr>';

if(!$numfail)
	print '<h3>Congratulations! Your server supports HostVise.</h3>';
else
	print '<h3>HostVise can run on your server, however the issue(s) above need to be corrected first.</h3>';
	
print '<h1><a href="http://hostvise.com/order" style="text-decoration:none; color:darkblue;">PURCHASE HOSTVISE!</a></h1>
Thanks for checking! Now use promo code <i>save10</i> to save 10% on your order.</div></body>';
?>