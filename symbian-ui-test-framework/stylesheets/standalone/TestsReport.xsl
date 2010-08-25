<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:param name="success_threshold" select="3" />

<xsl:template match="/tests">
	<html>
		<xsl:call-template name="head"/>
		<body onload="init()">
			<xsl:call-template name="banner"/>
			<div id="content">&#x0020;</div>
			<xsl:call-template name="report_info"/>
			<xsl:call-template name="brief_report"/>
			<xsl:call-template name="suites"/>
			<xsl:call-template name="detailed_report"/>
                        <xsl:call-template name="test_report"/>
		</body>
	</html>
</xsl:template>

<!-- Dump a report info table -->
<xsl:template name="report_info">
	<div id="report_info" class="hidden">
	<br/><br/>
	<xsl:if test="info/date or info/time or info/unitinfo">
	<P class="bigbig" align="center">.:: Information ::.</P>

	<table class="brief_report">
		<!-- DATE -->
		<xsl:if test="info/date">
		<tr>
			<td class="brief_report" align="right">
				Date :
			</td>
			<td class="brief_report" align="right">
		 		<xsl:value-of select="info/date/day"/>/<xsl:value-of select="info/date/month"/>/<xsl:value-of select="info/date/year"/>
			</td>
		</tr>
		</xsl:if>
		<!-- TIME -->
		<xsl:if test="info/time">
		<tr>
			<td class="brief_report" align="right">
				Time :
			</td>
			<td class="brief_report" align="right">
		 		<xsl:value-of select="info/time/hours"/>:<xsl:value-of select="info/time/minutes"/>:<xsl:value-of select="info/time/seconds"/>
			</td>
		</tr>
		</xsl:if>
		<!-- GENERAL INFO -->
		<xsl:for-each select="info/unitinfo">
		<tr>
			<td class="brief_report" align="right">
				<xsl:value-of select="name"/> :
			</td>
			<td class="brief_report" align="right">
		 		<xsl:value-of select="value"/>
			</td>
		</tr>
		</xsl:for-each>
	</table>
	<br/><br/>
	</xsl:if>
	</div>
</xsl:template>


<!-- Dump a brief report table -->
<xsl:template name="brief_report">
	<div id="brief_report" class="hidden">
	<TABLE class="brief_report">
		<!-- Success -->
		<TR>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				Succeeded tests &#x2713;:
			</TD>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="class">success</xsl:attribute>
		 		<xsl:value-of select="count(result[starts-with(status, 'Success')])"/>
			</TD>
		</TR>
		<!-- Mismatch -->
		<TR>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				Mismatched files count &#x2623;:
			</TD>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="class">mismatch</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'Mismatch')])"/>
			</TD>
		</TR>
		<!-- Failure -->
		<TR>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				Failed tests &#x2717;:
			</TD>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="class">failure</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'Failure')])"/>
			</TD>
		</TR>
		<!-- New -->
		<TR>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				New tests &#x2605;:
			</TD>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="class">new</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'New')])"/>
			</TD>
		</TR>
		<!-- Not run -->
		<TR>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				Not run tests &#x271d;:
			</TD>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="class">Notrun</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'Not run')])"/>
			</TD>
		</TR>
		<!-- Total -->
		<TR>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				Tests run:
			</TD>
			<TD class="brief_report">
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px;</xsl:attribute>
				<xsl:value-of select="count(result)"/>
			</TD>
		</TR>
	</TABLE>
	</div>
</xsl:template>

<!-- Print statitstics per test suite-->
<xsl:template name="suites">
	<div id="suites" class="hidden">
	<a name="list"/><P class="bigbig" align="center">.:: Tests Suites ::.</P>
	
	<table class="stats">
		<!-- Table head -->
		<thead bgcolor="#F0F0F0" >
		<tr class="stats">
			<th class="report">Test suite</th>
			<th class="report">Run tests</th>
			<th class="report">Succedeed tests</th>
			<th class="report">Failed tests</th>
			<th class="report">Success rate</th>		
		</tr>
		</thead>
		<!-- Table body -->
		<tbody>
		<xsl:for-each select="statistics/testsuite[string-length(name) &gt; 0]">
			<xsl:sort select="name" />
			<xsl:variable name="tsn" select="name"/>
			<!--xsl:if test="$tsn != ''"-->
				<TR>
					<td class="stats"><a>
						<xsl:attribute name="href">#</xsl:attribute>
						<xsl:attribute name="onclick">show('detail_<xsl:value-of select="name" />')</xsl:attribute>
						<strong><xsl:value-of select="name"/></strong>
					</a></td>
					<td class="stats" align="center"><xsl:value-of select="runtests"/></td>
					<td class="stats" align="center"><xsl:value-of select="passedtests"/></td>
					<td class="stats" align="center"><xsl:value-of select="failedtests"/></td>
					<xsl:choose>
						<xsl:when test="successrate = 100">
						<td class="success" align="center"><b><xsl:value-of select="successrate"/></b></td>
						</xsl:when>
						<xsl:otherwise>
						<td class="failure" align="center"><b><xsl:value-of select="successrate"/></b></td>
						</xsl:otherwise>
					</xsl:choose>
				</TR>
			<!--/xsl:if-->
		</xsl:for-each>	
		<!-- Table foot -->
		<tr>
			<td class="stats"><b>Total</b></td>
			<td class="stats" align="center"><b><xsl:value-of select="statistics/total/runtests"/></b></td>
			<td class="stats" align="center"><b><xsl:value-of select="statistics/total/passedtests"/></b></td>
			<td class="stats" align="center"><b><xsl:value-of select="statistics/total/failedtests"/></b></td>
			<xsl:choose>
				<xsl:when test="statistics/total/successrate = 100">
				<td class="success" align="center"><b><xsl:value-of select="statistics/total/successrate"/></b></td>
				</xsl:when>
				<xsl:otherwise>
				<td class="failure" align="center"><b><xsl:value-of select="statistics/total/successrate"/></b></td>
				</xsl:otherwise>
			</xsl:choose>
		</tr>
		</tbody>
	</table>
	</div>
</xsl:template>	


<!-- Print real report -->
<xsl:template name="detailed_report">
	<!-- Sort results by category / name -->
	<xsl:for-each select="result[not(category = preceding-sibling::result/category)]">
		<xsl:sort select="category"/>
		<xsl:variable name="cat" select="category"/>
		<xsl:if test="$cat != ''">
			<div class="hidden">
				<xsl:attribute name="id">detail_<xsl:value-of select="category"/></xsl:attribute>
				<P class="bigbig" align="center">.:: Detailed Report for ' <xsl:value-of select="category"/> '::.</P>
				
				<table align="center" width="90%">
				<tr><th class="report">Test</th><th class="report">Status</th></tr>
				<xsl:apply-templates select="/tests/result[category=$cat]">
					<xsl:sort select="name" />
				</xsl:apply-templates>
				</table>
			</div>
		</xsl:if>
	</xsl:for-each>
</xsl:template>

<!-- Print test report -->
<xsl:template name="test_report">
	<xsl:for-each select="result">
		<xsl:variable name="test_name" select="name"/>
		<xsl:if test="$test_name != ''">
			<div class="hidden">
				<xsl:attribute name="id">log_<xsl:value-of select="name"/></xsl:attribute>
				<P class="bigbig" align="center">.:: Detailed report for ' <xsl:value-of select="category"/> / <xsl:value-of select="name"/> '::.</P>
                                    <table align="center" border="1" cellspacing="1" cellpadding="3">
                                    <tr>
                                        <td>
                                            <b>Status:</b>
                                        </td>
                                            <xsl:choose>
                                                <!-- If the result is a digit value and the test is a failure-->
                                                <xsl:when test="value and starts-with(status, 'Failure')">
                                                <!-- Actual result -->
                                                <td>
                                                    <xsl:attribute name="align">center</xsl:attribute>
                                                    <xsl:attribute name="style">
                                                            <xsl:call-template name="style">
                                                                    <xsl:with-param name="status" />
                                                                    <xsl:with-param name="diffavg" />
                                                            </xsl:call-template>
                                                    </xsl:attribute>
                                                    <xsl:value-of select="value/got"/> (Ref:<xsl:value-of select="value/ref"/>)
                                                </td>
                                                </xsl:when>
                                                <!-- If the result is NOT a digit value or if the test is successful-->
                                            <xsl:otherwise>
                                                <!-- Status -->
                                                <td>
                                                    <xsl:attribute name="align">center</xsl:attribute>
                                                    <xsl:attribute name="style">
                                                            <xsl:call-template name="style">
                                                                    <xsl:with-param name="status" />
                                                                    <xsl:with-param name="diffavg" />
                                                            </xsl:call-template>
                                                    </xsl:attribute>
                                                    <xsl:call-template name="status">
                                                            <xsl:with-param name="status" />
                                                            <xsl:with-param name="diffavg" />
                                                    </xsl:call-template>
                                                </td>
                                            </xsl:otherwise>
                                            </xsl:choose>                
                                    </tr>
                                    </table>
                                <br />
                                <ul>
                                    <li>
                                        <a>
                                        <xsl:attribute name="href">#</xsl:attribute>
                                        <xsl:attribute name="onclick">show('detail_<xsl:value-of select="category" />')</xsl:attribute>
                                        Return to detailed report ' <xsl:value-of select="category"/> '
                                        </a>
                                    </li>
                                    <br /><br />
                                    <li>
                                    Comments:
                                    <pre><xsl:value-of select="file/comments"/></pre>
                                    </li>
                                    <br />
                                    <xsl:if test="file/loggot">
                                    <li>
                                        Logs:<br />
                                        <iframe align="center" width="960px" height="480px">
                                        <xsl:attribute name="src"><xsl:value-of select="file/loggot"/></xsl:attribute>
                                        </iframe>
                                        <br/>
                                    </li>
                                    </xsl:if>
                                    <xsl:if test="file/logref">
                                    <li>
                                        Ref Logs:<br />
                                        <iframe align="center" width="960px" height="480px">
                                        <xsl:attribute name="src"><xsl:value-of select="file/logref"/></xsl:attribute>
                                        </iframe>
                                        <br/>
                                    </li>
                                    </xsl:if>
                                </ul>
			</div>
		</xsl:if>
	</xsl:for-each>
</xsl:template>

<!-- Print result -->
<xsl:template match="result">
	<xsl:variable name="n" select="generate-id(name)"/>
	<tr>
	<xsl:choose>
	<!-- If the result is a digit value and the test is a failure-->
	<xsl:when test="value">
		<!-- Test name -->
		<td>
			<xsl:attribute name="width">640px</xsl:attribute>
			<xsl:attribute name="align">left</xsl:attribute>
			<xsl:attribute name="class">report</xsl:attribute>
                    <a>
                    <xsl:attribute name="href">#</xsl:attribute>
                    <xsl:attribute name="onclick">show('log_<xsl:value-of select="name" />')</xsl:attribute>
			<B><xsl:value-of select="position()"/></B>: 
			<xsl:if test="string-length(prologue) &gt; 0"><B><xsl:value-of select="prologue"/></B> / </xsl:if>
			<xsl:if test="string-length(category) &gt; 0"><B><xsl:value-of select="category"/></B> / </xsl:if>
			<B><xsl:value-of select="name"/></B>
                    </a>
		</td>
            <xsl:choose>
            <xsl:when test="starts-with(status, 'Failure')">
		<!-- Actual result -->
		<td>
                        <xsl:attribute name="width">256px</xsl:attribute>
                        <xsl:attribute name="nowrap">nowrap</xsl:attribute>
			<xsl:attribute name="align">center</xsl:attribute>
			<xsl:attribute name="style">
				<xsl:call-template name="style">
					<xsl:with-param name="status" />
					<xsl:with-param name="diffavg" />
				</xsl:call-template>
			</xsl:attribute>
			<xsl:attribute name="class">report</xsl:attribute>
			<xsl:value-of select="value/got"/> (Ref:<xsl:value-of select="value/ref"/>)
		</td>
	</xsl:when>
            <xsl:otherwise>
                 <td>
                    <xsl:attribute name="width">256px</xsl:attribute>
                    <xsl:attribute name="nowrap">nowrap</xsl:attribute>
                    <xsl:attribute name="align">center</xsl:attribute>
                    <xsl:attribute name="style">
                            <xsl:call-template name="style">
                                    <xsl:with-param name="status" />
                                    <xsl:with-param name="diffavg" />
                            </xsl:call-template>
                    </xsl:attribute>
                    <xsl:attribute name="class">report</xsl:attribute>
                    <xsl:call-template name="status">
                            <xsl:with-param name="status" />
                            <xsl:with-param name="diffavg" />
                    </xsl:call-template>
                </td>
            </xsl:otherwise>
            </xsl:choose>
	</xsl:when>
	<!-- If the result is NOT a digit value or if the test is successful-->
	<xsl:otherwise>
		<!-- Test name -->
		<td>
			<xsl:attribute name="width">640px</xsl:attribute>
			<xsl:attribute name="align">left</xsl:attribute>
			<xsl:attribute name="class">report</xsl:attribute>
			<B><xsl:value-of select="position()"/></B>: 
			<xsl:if test="string-length(prologue) &gt; 0"><B><xsl:value-of select="prologue"/></B> / </xsl:if>
			<xsl:if test="string-length(category) &gt; 0"><B><xsl:value-of select="category"/></B> / </xsl:if>
			<B><xsl:value-of select="name"/></B>
		</td>
		<!-- Status -->
		<td>
			<xsl:attribute name="width">256px</xsl:attribute>
			<xsl:attribute name="nowrap">nowrap</xsl:attribute>
			<xsl:attribute name="align">center</xsl:attribute>
			<xsl:attribute name="style">
				<xsl:call-template name="style">
					<xsl:with-param name="status" />
					<xsl:with-param name="diffavg" />
				</xsl:call-template>
			</xsl:attribute>
			<xsl:attribute name="class">report</xsl:attribute>
			<xsl:call-template name="status">
				<xsl:with-param name="status" />
				<xsl:with-param name="diffavg" />
			</xsl:call-template>
		</td>
	</xsl:otherwise>	
	</xsl:choose>
	</tr>
</xsl:template>	


<!-- Returns color info for success or failure -->
<xsl:template name="style">
	<xsl:param name="status" />
	<xsl:param name="diffavg" />
	<xsl:choose>
		<xsl:when test="starts-with(status, 'Success')">
			font-weight:bolder; background-color:#22aa22; color:white;
		</xsl:when>
		<xsl:when test="starts-with(status, 'Failure')">

			<xsl:choose>
				<xsl:when test="diffavg &gt; $success_threshold">
					font-weight:bolder; background-color:#ff7770; color:white; 
				</xsl:when>
				<xsl:otherwise>
					font-weight:bolder; background-color:#77aa22; color:white;
				</xsl:otherwise>
			</xsl:choose>

		</xsl:when>
		<xsl:when test="starts-with(status, 'Mismatch')">
			font-weight:bolder; background-color:orange; color:white;
		</xsl:when>
		<xsl:when test="starts-with(status, 'New')">
			font-weight:bolder; background-color:#778fe5; color:white;
		</xsl:when>
		<xsl:when test="starts-with(status, 'Not run')">
			font-weight:bolder; background-color:#707070; color:white;
		</xsl:when>
		<xsl:otherwise>
			font-weight:bolder;
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<!-- Return status string -->
<xsl:template name="status">
	<xsl:param name="status" />
	<xsl:param name="diffavg" />
	<xsl:choose>
		<xsl:when test="starts-with(status, 'Success')">
			<xsl:value-of select="status"/> &#x2713;
		</xsl:when>
		<xsl:when test="starts-with(status, 'Failure')">

			<xsl:choose>
				<xsl:when test="diffavg &gt; $success_threshold">
					<xsl:value-of select="status"/> (<xsl:value-of select="diffavg"/>%) &#x2717;
				</xsl:when>
				<xsl:otherwise>
					Almost (<xsl:value-of select="diffavg"/>%) &#x2713;
				</xsl:otherwise>
			</xsl:choose>

		</xsl:when>
		<xsl:when test="starts-with(status, 'Mismatch')">
			<xsl:value-of select="status"/> &#x2623;
		</xsl:when>
		<xsl:when test="starts-with(status, 'New')">
			<xsl:value-of select="status"/> &#x2605;
		</xsl:when>
		<xsl:when test="starts-with(status, 'Not run')">
			<xsl:value-of select="status"/> &#x271d;
		</xsl:when>
		<xsl:otherwise>
			<xsl:value-of select="status"/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="banner">
	<table>
		<xsl:attribute name="class">borderless_table</xsl:attribute>
		<xsl:attribute name="width">95%</xsl:attribute>
		<xsl:attribute name="align">center</xsl:attribute>
		<TR>
			<!-- Title -->
			<TD>
				<xsl:attribute name="class">title</xsl:attribute>
				<xsl:attribute name="nowrap"/>
				<xsl:apply-templates select="/tests/info/title"/>
			</TD>
			<TD>
				<xsl:attribute name="align">left</xsl:attribute>
				<xsl:attribute name="valign">center</xsl:attribute>
				<!--xsl:apply-templates select="/tests/info/url" /-->
				<div id="banner">&#x0020;</div>
			</TD>
		</TR>
	</table>
</xsl:template>

<xsl:template name="head">
	<head>
	<title><xsl:apply-templates select="/tests/info/title"/></title>
	<xsl:call-template name="css"/>
	<xsl:call-template name="script"/>
	</head>
</xsl:template>

<!-- Print an URL -->
<xsl:template match="url">
	<!-- URL -->
	&#x27a8; <A>
		<xsl:attribute name="href"><xsl:value-of select="value" /></xsl:attribute>
		<xsl:value-of select="name" />
	</A><br />
</xsl:template>

<xsl:template name="css">
<style type="text/css">
<![CDATA[
/* page style */
body {
	font-family : 'Arial Unicode MS', Verdana, Arial, Helvetica, sans-serif;
	/*background-color : rgb(250,244,232);*/
	background-color : rgb(232,238,247);
	font-size : small;
	color:black;
}

/* table style */
table {
	font-family : 'Arial Unicode MS', Verdana, Arial, Helvetica, sans-serif;
	border-collapse: collapse;
	/*background-color:white;*/
	background-color: rgb(195,217,255);
	font-size : small;
	color:black;
}

.hidden {
	display: none;
}
td.brief_report, th.brief_report, td.stats, th.stats {
	border:1px solid black;
	border-collapse:collapse;
}
a {
	color: #000000;
}

.borderless_table {
	background-color : rgb(232,238,247);
	border: 0px #FAF4E8 solid;
	border-collapse:collapse;
}

.title {
	font-size:24pt;
	font-weigth:bolder;
	text-align:center;
	vertical-align:center;
}

.bigbig {
	font-size: large;
	font-weight: bolder;
}

/********** when writing a report ************/
.report {
	border:1px solid black;
	border-collapse:collapse;
	font-family:'Arial Unicode MS', Verdana, Arial, Helvetica, sans-serif;
	font-size:small;
}
.brief_report {
	border:1px solid black;
	border-collapse:collapse;
	font-family:'Arial Unicode MS', Verdana, Arial, Helvetica, sans-serif;
	font-size:small;
}

.stats {
	border:1px solid black;
	border-collapse:collapse;
	font-family:'Arial Unicode MS', Verdana, Arial, Helvetica, sans-serif;
	font-size:small;
}

/* a wide report table */
table.report {
	position:relative;
	left: 50%;
	width: 960px;
	margin-left: -480px;
}

/* a narrow report table */
table.brief_report {
	position:relative;
	left: 50%;
	width: 400px;
	margin-left: -200px;
}

/* a medium report table */
table.stats {
	position:relative;
	left: 50%;
	width: 600px;
	margin-left: -300px;
}

th.report	{ font-weight:bolder; background-color:rgb(181,237,188); color:black; border:1px solid black; border-collapse:collapse; }
/* test status */
td.success	{ font-weight:bolder; background-color:rgb(34,170,34); color:white; border:1px solid black; border-collapse:collapse; }
td.failure	{ font-weight:bolder; background-color:rgb(255,119,112); color:white; border:1px solid black; border-collapse:collapse; }
td.mismatch	{ font-weight:bolder; background-color:rgb(240,190,0); color:white; border:1px solid black; border-collapse:collapse; }
td.new		{ font-weight:bolder; background-color:#778fe5; color:white; border:1px solid black; border-collapse:collapse; }
td.Notrun	{ font-weight:bolder; background-color:#707070; color:white; border:1px solid black; border-collapse:collapse; }
td.almost	{ font-weight:bolder; background-color:#66a011; color:white; border:1px solid black; border-collapse:collapse; }

/* those do not work in IE, even the latest version */
/*
td.success:after	{ content: ' ✓'; }
td.failure:after	{ content: ' ✗'; }
td.mismatch:after	{ content: ' ☣'; }
td.new:after		{ content: ' ★'; }
td.Notrun:after	{ content: ' ✝'; }*/

]]>
</style>
</xsl:template>

<xsl:template name="script">
<script type="text/javascript">
<![CDATA[
<!--
if (navigator.appName == 'Netscape') 
	var langcode = navigator.language; 
else 
	var langcode = navigator.browserLanguage;

var lang='english';
if (langcode.indexOf('de') > -1) lang='german';
else if (langcode.indexOf('fr') > -1) lang='french';
else if (langcode.indexOf('ja') > -1) lang='japanese';
else if (langcode.indexOf('fi') > -1) lang='finnish';

function show(div) {
		var content = document.getElementById("content");
		var banner   = document.getElementById("banner");
		var suite   = document.getElementById(div);
		
		content.innerHTML = suite.innerHTML;
		banner.innerHTML='&#x27a8; <A href="javascript:showTOC()"><strong>Return to Main Page</strong></A><br />';
}

function showTOC() {
		var content = document.getElementById("content");
		var info	= document.getElementById("report_info");
		var brief   = document.getElementById("brief_report");
		var suites  = document.getElementById("suites");
		content.innerHTML = info.innerHTML+brief.innerHTML+suites.innerHTML;

		var banner   = document.getElementById("banner");
		banner.innerHTML='&#x27a8; <A href="#list"><strong>Select a Tests Suite from the list</strong></A><br />';
}

function localFileExists(fileName) {
	if ((typeof ActiveXObject) == 'function') {
		// IE
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		return fso.FileExists(fileName);
	}
	else {
		// Mozilla/Firefox
		netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
		var file = Components.classes["@mozilla.org/file/local;1"].createInstance(Components.interfaces.nsILocalFile);
		file.initWithPath(fileName);
		return file.exists();
	}
	return false;
}

function init() {
	// TODO: localization support...
	showTOC();
}
//-->
]]>
</script>
<noscript>
	<strong>Javascript must be enabled!</strong>
</noscript>
</xsl:template>

</xsl:stylesheet>

