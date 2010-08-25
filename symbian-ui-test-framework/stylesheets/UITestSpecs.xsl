<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!-- Stylesheet parameters -->
<xsl:param name="test_name" />		<!-- test name	-->

<!-- Main entry point -->
<xsl:template match="tests">
	<HTML>
	<!-- HTML header -->
	<xsl:call-template name="head" />

	<!-- HTML body -->
	<BODY>

	<!-- detailed report -->
	<xsl:call-template name="detailed_report" />

	</BODY>
	</HTML>
</xsl:template>

<!-- ================= Start of HTML header ================= -->

<xsl:template name="head">
	<HEAD>
		<!-- Report title -->
		<TITLE><xsl:apply-templates select="info/title"/></TITLE>
		<!-- CSS stylesheet -->
		<xsl:call-template name="css" />
		<!-- Javascript stylesheet -->
		<xsl:call-template name="javascript" />		
	</HEAD>
</xsl:template>

<xsl:template name="css">
	<style type="text/css">
		pre	{
			font-family: 'Lucida Console', Courier, 'Courier New', fixed;
			font-size: x-small;
			color: #884400;
			background-color: #FAFAFA;
			border: #DCD7D1;
			border-style: solid;
			border-left-width: 1px;
			border-top-width: 1px;
			border-right-width: 1px;
			border-bottom-width: 1px
		}
		p	{
			font-family: 'Arial Unicode MS',Verdana,Arial,Helvetica,sans-serif;
		}
		body	{
			font-family: 'Arial Unicode MS',Verdana,Arial,Helvetica,sans-serif;
			background-color: #FAF4E8;
			font-size: small;
		}
		table	{
			font-family: 'Arial Unicode MS',Verdana,Arial,Helvetica,sans-serif;
			font-size: small;
			background-color: white;
			border: 1px black solid;
			border-collapse:collapse;
		}
		tr {
			border: 1px black solid;
			border-collapse:collapse;
		}
		td {
			border: 1px black solid;
			border-collapse:collapse;
		}
		.testhead {
			border: 0px black solid;
			border-collapse:collapse;
		}
		.borderless_table {
			background-color: #FAF4E8;
			border: 0px #FAF4E8 solid;
			border-collapse:collapse;
		}
		.details {
			color: #000000;
		}
		.chapter {
			font-size: large;
			font-weight: bolder;
		}
		a {
			color: #000000;
		}
	</style>
</xsl:template>

<xsl:template name="javascript">
	<xsl:text disable-output-escaping="yes">
	&lt;script language="javascript">
		
		var timerId = 0;
		
		function updateimg(imgname) {
			if (timerId) {
				clearTimeout(timerId);
				timerId = 0;
			}
			if (document.images) {
				var toto=document.images[''+imgname].src;
				if (toto) {
					document.images[''+imgname].src = '/autobuilds/expand.jpg' + '&amp;' + Math.random();
					document.images[''+imgname].src = '' + toto + '&amp;' + Math.random();
				}
				else {
					alert('This browser is not supported!');
				}
			}
		}
		
		function helpLog() {
			var referer=window.location.href;
			var query = '/autobuilds/refmgr.cgi?action=help&amp;referer='+referer;
			window.open(query, "Help", "fullscreen=no,toolbar=no,status=no,menubar=no,scrollbars=yes,resizable=yes,directories=no,location=no,width=800");
		}
		
		function viewLog() {
			var referer=window.location.href;
			var query = '/autobuilds/refmgr.cgi?action=view&amp;referer='+referer;
			window.open(query, "View", "fullscreen=no,toolbar=no,status=no,menubar=no,scrollbars=yes,resizable=no,directories=no,location=no,width=800");
		}
		function downloadZip() {
			var referer=window.location.href;
			var query = '/autobuilds/refmgr.cgi?action=download&amp;referer='+referer;
			window.open(query, "View", "fullscreen=no,toolbar=no,status=no,menubar=no,scrollbars=no,resizable=no,directories=no,location=no,width=800");
		}
		function reloadall(referer) {
			if (timerId) {
				clearTimeout(timerId);
				timerId = 0;
			}
			window.location.href=referer;
		}
		function clearLog() {
			var referer=window.location.href;
			var query = '/autobuilds/refmgr.cgi?action=clear&amp;referer='+referer;
			window.open(query, "Clear", "fullscreen=no,toolbar=no,status=no,menubar=no,scrollbars=yes,resizable=no,directories=no,location=no,width=800");
			
			timerId = setTimeout("reloadall('"+referer+"')", 2000);
		}
		
		function commitLog() {
			var referer=window.location.href;
			var query = '/autobuilds/refmgr.cgi?action=commit&amp;referer='+referer;
			window.open(query, "Commit", "fullscreen=no,toolbar=no,status=no,menubar=no,scrollbars=yes,resizable=no,directories=no,location=no,width=800");
		}
		
		function suubmit(myform, imgname) {
			var referer=window.location.href;
			var query = '/autobuilds/refmgr.cgi?action=' + myform.elements['action'].value + '&amp;file=' + myform.elements['file'].value + '&amp;revert=' + myform.elements['revert'].value + '&amp;referer='+referer;
			window.open(query, "Status", "fullscreen=no,toolbar=no,status=no,menubar=no,scrollbars=yes,resizable=no,directories=no,location=no,width=600,height=256");
			
			timerId = setTimeout("updateimg('"+imgname+"')", 1000);
			return true;
		}

		function suubmitall(myform, imgname) {
			alert(myform.elements['revert'].value+'  '+imgname);
			return true;
		}

	&lt;/script>
	</xsl:text>
</xsl:template>

<!-- ================= End of HTML header ================= -->


<!-- ================= Start of HTML body ================= -->


<!-- Dump a brief report table -->
<xsl:template name="brief_report">
	<P class="chapter" align="center">.:: Brief Report ::.</P>
	<TABLE align="center" width="40%">
		<!-- Success -->
		<TR>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				Tests succeeded &#x2713;:
			</TD>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px; background-color:#22aa22; color:white;</xsl:attribute>
		 		<xsl:value-of select="count(result[starts-with(status, 'Success')])"/>
			</TD>
		</TR>
		<!-- Mismatch -->
		<TR>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				Mismatched files count &#x2623;:
			</TD>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px; background-color:orange; color:white;</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'Mismatch')])"/>
			</TD>
		</TR>
		<!-- Failure -->
		<TR>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				Tests failed &#x2717;:
			</TD>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px; background-color:#ff7770; color:white;</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'Failure')])"/>
			</TD>
		</TR>
		<!-- New -->
		<TR>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				New tests &#x2605;:
			</TD>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px; background-color:#778fe5; color:white;</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'New')])"/>
			</TD>
		</TR>
		<!-- Not run -->
		<TR>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				Not run tests &#x271d;:
			</TD>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px; background-color:#707070; color:white;</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'Not run')])"/>
			</TD>
		</TR>
		<!-- Total -->
		<TR>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				Tests run:
			</TD>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px;</xsl:attribute>
				<xsl:value-of select="count(result)"/>
			</TD>
		</TR>
	</TABLE>
</xsl:template>

<!-- Print real report -->
<xsl:template name="detailed_report">
	<!-- Sort results by prologue / category / name -->
	<xsl:apply-templates select="result">
		<xsl:sort select="prologue" />
		<xsl:sort select="category" />
	</xsl:apply-templates>
</xsl:template>


<!-- Print result -->
<xsl:template match="result">
	<xsl:variable name="n" select="generate-id(name)"/>
	<xsl:variable name="nameTest" select="position()"/>
	<!-- Only print result if it's part of the current page -->
	<xsl:if test="$nameTest = $test_name">
		<B><xsl:value-of select="name"/></B>
		<P>
		<TABLE>
			<xsl:attribute name="border">0</xsl:attribute>
			<xsl:attribute name="width">100%</xsl:attribute>
			<!-- Headers -->
			<TR>
				<TD>
					<xsl:attribute name="align">center</xsl:attribute>
					<xsl:attribute name="style">font-weight:bolder;</xsl:attribute>
					#
				</TD>
				<TD>
					<xsl:attribute name="width">256px</xsl:attribute>
					<xsl:attribute name="align">center</xsl:attribute>
					<xsl:attribute name="style">font-weight:bolder;</xsl:attribute>
					Action
				</TD>
				<TD>
					<xsl:attribute name="align">center</xsl:attribute>
					<xsl:attribute name="style">font-weight:bolder;</xsl:attribute>
					Screenshot result
				</TD>
			</TR>
			<!-- Files -->
			<xsl:apply-templates select="file">
				<xsl:sort select="name" />
			</xsl:apply-templates>
		</TABLE>
		</P>
	</xsl:if>
</xsl:template>	

<!-- Print file comparison result -->
<xsl:template match="file">
	<xsl:variable name="r" select="ref"/>								<!-- Reference file		-->
	<xsl:variable name="g" select="got"/>							<!-- Actual file			-->
	<xsl:variable name="d" select="diff"/>							<!-- Difference result	-->
	<xsl:variable name="nm" select="name"/>						<!-- File name			-->
	<xsl:variable name="tm" select="generate-id(name)"/>		<!-- File name id		-->
	<xsl:variable name="tn" select="generate-id(../name)"/>	<!-- Test name id		-->
	<TR>
		<!-- File index -->
		<TD>
			<xsl:attribute name="width">32px</xsl:attribute>
			<xsl:attribute name="align">center</xsl:attribute>
			<xsl:number value="position()-1" format="1" /><br/>
		</TD>
		<!-- action -->
		<TD>
			<xsl:attribute name="align">center</xsl:attribute>
			<xsl:attribute name="valign">center</xsl:attribute>
			<!-- Reference -->
			<xsl:value-of select="comments"/>
		</TD>
		<!-- Screenshot -->
		<TD>
			<xsl:attribute name="align">center</xsl:attribute>
			<xsl:attribute name="valign">center</xsl:attribute>
			<img src="{$r}" width="320px" title="Reference screenshot" border="0" style="display:block" />
		</TD>
	</TR>
</xsl:template>	

<!-- Returns color info for success or failure -->
<xsl:template name="style">
	<xsl:param name="status" />
	<xsl:choose>
		<xsl:when test="starts-with(status, 'Success')">
			font-weight:bolder; background-color:#22aa22; color:white;
		</xsl:when>
		<xsl:when test="starts-with(status, 'Failure')">
			font-weight:bolder; background-color:#ff7770; color:white;
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

<!-- Ditto, but based on diff value -->
<xsl:template name="style_screen">
	<xsl:param name="diffvalue" />
	<xsl:choose>
		<xsl:when test="diffvalue &gt; 0">
			font-weight:bolder; background-color:#ff7770; color:white;
		</xsl:when>
		<xsl:otherwise>
			font-weight:bolder; background-color:#22aa22; color:white;
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>



<!-- Print an URL -->
<xsl:template match="url">
	<!-- URL -->
	&#x27a8; <A>
		<xsl:attribute name="href"><xsl:value-of select="value" /></xsl:attribute>
		<xsl:value-of select="name" />
	</A><br />
</xsl:template>

<!-- ================= End of HTML body ================= -->


<!-- ================= Start of optional templates ================= -->

<xsl:template name="optional">
	<!-- Files -->
	<!--P class="chapter" align="center">.:: Files ::.</P>

	<TABLE align="center">
		<TR>
			<TD align="center" style="font-weight:bolder;">Name</TD>
			<TD align="center" style="font-weight:bolder;">Description</TD>
		</TR>
		<TR>
			<TD><a class="details" href="reference.zip">&#x21e9; Reference.zip</a></TD>
			<TD>Single archive containing all screenshots in PNG format.</TD>
		</TR>
		<TR>
			<TD><a class="details" href="results.xml">&#x21e9; Results.xml</a></TD>
			<TD>Results in XML format.</TD>
		</TR>
		<TR>
			<TD><a class="details" href="results.xsl">&#x21e9; Results.xsl</a></TD>
			<TD>Current XML Stylesheet.</TD>
		</TR>
	</TABLE-->
</xsl:template>

</xsl:stylesheet>
