<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!-- Stylesheet parameters -->
<xsl:param name="mode" />				<!-- 0=show tests, 1=show test's files, 2=show statistics -->
<xsl:param name="test_name" />			<!-- test name	-->

<xsl:param name="start" />				<!-- first result in the page --> 
<xsl:param name="end" />				<!-- last result in the page -->
<xsl:param name="current_page" />		<!-- current page number -->
<xsl:param name="num_pages" />			<!-- number of pages -->
<xsl:param name="sucess_threshold" />	<!-- sucess threshold -->
<xsl:param name="testsuite" /> 			<!-- testsuite treated -->

<!-- Main entry point -->
<xsl:template match="tests">
	<HTML>
	<!-- HTML header -->
	<xsl:call-template name="head" />

	<!-- HTML body -->
	<BODY>

	<!-- Title and optional links -->
	<TABLE>
		<xsl:attribute name="class">borderless_table</xsl:attribute>
		<xsl:attribute name="width">95%</xsl:attribute>
		<xsl:attribute name="align">center</xsl:attribute>
		<TR>
			<!-- Title -->
			<TD>
				<xsl:attribute name="class">borderless_table</xsl:attribute>
				<!--xsl:attribute name="width">50%</xsl:attribute-->
				<xsl:attribute name="style">font-size:24pt; font-weigth:bolder; font-family: Verdana,Arial,Helvetica,sans-serif;</xsl:attribute>
				<xsl:attribute name="align">center</xsl:attribute>
				<xsl:attribute name="valign">center</xsl:attribute>
				<xsl:apply-templates select="info/title"/>
			</TD>
			<TD>
				<xsl:attribute name="class">borderless_table</xsl:attribute>
				<!--xsl:attribute name="width">50%</xsl:attribute-->
				<xsl:attribute name="align">left</xsl:attribute>
				<xsl:attribute name="valign">top</xsl:attribute>
				<xsl:apply-templates select="info/url" />
			</TD>
		</TR>
	</TABLE>

	<xsl:choose>
		<!-- Normal mode -->
		<xsl:when test="$mode = 0">
			<!-- optional templates. You can modify these. -->
			<xsl:call-template name="optional" />

			<!-- brief report summary -->
			<xsl:call-template name="brief_report" />

			<!-- Page number -->
			<xsl:if test="$num_pages &gt; 1">
				<P class="chapter" align="center">
					Page <xsl:value-of select="$current_page" /> of <xsl:value-of select="$num_pages" />
				</P>
			</xsl:if>

			<!-- Navigation bar -->
			<xsl:call-template name="navigation" />

			<!-- detailed report -->
			<xsl:call-template name="detailed_report" />
			<HR />
			
			<!-- Navigation bar (again) -->
			<xsl:call-template name="navigation" />
		</xsl:when>
		<!-- Test inner mode -->
		<xsl:otherwise>
			<xsl:choose>
				<xsl:when test="$mode = 1">
					<!-- detailed report -->
					<xsl:call-template name="detailed_report" />
				</xsl:when>
				<xsl:otherwise>
					<!-- report info -->
					<xsl:call-template name="report_info" />
					<!-- brief report summary -->
					<xsl:call-template name="brief_report" />
					<!-- Statistics -->
					<xsl:call-template name="statistics" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:otherwise>
	</xsl:choose>
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
		function ShowShots(name) {
			var url=name+'.html';
			window.open(url,"popUp","status=no, scrollbars=yes, left=60, top=10"); 
		}
		function hideIt(name) {
			var i = document.getElementById(name);
			i.style.display="none";
		}
		
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
			
			if (imgname == '') {
				timerId = setTimeout("reloadall('"+referer+"')", 2000);
			}
			else {
				timerId = setTimeout("updateimg('"+imgname+"')", 1000);
			}
			return true;
		}

	&lt;/script>
	</xsl:text>
</xsl:template>

<!-- ================= End of HTML header ================= -->


<!-- ================= Start of HTML body ================= -->

<!-- Print the navigation bar -->
<xsl:template name="navigation">
	<xsl:if test="$num_pages &gt; 1">
		<P ALIGN="center">
	
		<!-- Previous page link -->
		<xsl:choose>
			<xsl:when test="$current_page &gt; 2">
				<A>
					<xsl:attribute name="class">details</xsl:attribute>
					<xsl:attribute name="href"><xsl:value-of select="$testsuite"/><xsl:value-of select="$current_page - 1"/>.html</xsl:attribute>
					<b>Previous</b>
				</A>
				&#x007c;
			</xsl:when>
			<xsl:when test="$current_page = 2">
				<A>
					<xsl:attribute name="class">details</xsl:attribute>
					<xsl:attribute name="href"><xsl:value-of select="$testsuite"/>.html</xsl:attribute>
					<b>Previous</b>
				</A>
				&#x007c;
			</xsl:when>
		</xsl:choose>

		<!-- Print all the page numbers with links (recursive) -->
		<xsl:call-template name="recurse_nav">
			<xsl:with-param name="pagenum" select="1"/>
		</xsl:call-template>

		<!-- Previous page link -->
		<xsl:choose>
			<xsl:when test="$current_page &lt; $num_pages">
				&#x007c;
				<A>
					<xsl:attribute name="class">details</xsl:attribute>
					<xsl:attribute name="href"><xsl:value-of select="$testsuite"/><xsl:value-of select="$current_page + 1"/>.html</xsl:attribute>
					<b>Next</b>
				</A>
			</xsl:when>
		</xsl:choose>
		
		</P>
	</xsl:if>
</xsl:template>

<!-- Recursive utility method for page numbers -->
<xsl:template name="recurse_nav">
	<xsl:param name="pagenum" />
	<xsl:if test="$pagenum &lt;= $num_pages">
		<!-- Print separator -->
		<xsl:if test="$pagenum &gt; 1"> &#x007c; </xsl:if>
		
		<xsl:choose>
			<!-- Page is not current: let it be a link -->
			<xsl:when test="$current_page != $pagenum">
				<xsl:choose>
					<xsl:when test="$pagenum &gt; 1">
						<A>
						<xsl:attribute name="class">details</xsl:attribute>
						<xsl:attribute name="href"><xsl:value-of select="$testsuite"/><xsl:value-of select="$pagenum"/>.html</xsl:attribute>
						<b><xsl:value-of select="$pagenum"/></b>
						</A>
					</xsl:when>
					<xsl:otherwise>
						<A>
						<xsl:attribute name="class">details</xsl:attribute>
						<xsl:attribute name="href"><xsl:value-of select="$testsuite"/>.html</xsl:attribute>
						<b>1</b>
						</A>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<!-- Page is current: no link -->
			<xsl:otherwise>
				<b><xsl:value-of select="$pagenum"/></b>
			</xsl:otherwise>
		</xsl:choose>
		
		<!-- Recurse -->
		<xsl:call-template name="recurse_nav">
			<xsl:with-param name="pagenum" select="$pagenum + 1"/>
		</xsl:call-template>
		
	</xsl:if>
</xsl:template>

<!-- Dump a report info table -->
<xsl:template name="report_info">
	<xsl:if test="info/date or info/time or info/unitinfo">
	<P class="chapter" align="center">.:: Information ::.</P>

	<table align="center">
		<!-- DATE -->
		<xsl:if test="info/date">
		<tr>
			<td align="right">
				Date :
			</td>
			<td align="right">
		 		<xsl:value-of select="info/date/day"/>/<xsl:value-of select="info/date/month"/>/<xsl:value-of select="info/date/year"/>
			</td>
		</tr>
		</xsl:if>
		<!-- TIME -->
		<xsl:if test="info/time">
		<tr>
			<td align="right">
				Time :
			</td>
			<td align="right">
		 		<xsl:value-of select="info/time/hours"/>:<xsl:value-of select="info/time/minutes"/>:<xsl:value-of select="info/time/seconds"/>
			</td>
		</tr>
		</xsl:if>
		<!-- GENERAL INFO -->
		<xsl:for-each select="info/unitinfo">
		<tr>
			<td align="right">
				<xsl:value-of select="name"/> :
			</td>
			<td align="right">
		 		<xsl:value-of select="value"/>
			</td>
		</tr>
		</xsl:for-each>
	</table>
	</xsl:if>
</xsl:template>

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
		<!-- Not implemented -->
		<TR>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				Not implemented &#x271d;:
			</TD>
			<TD>
				<xsl:attribute name="align">right</xsl:attribute>
				<xsl:attribute name="style">font-weight:bolder; width:128px; background-color:#AAAAAA; color:white;</xsl:attribute>
				<xsl:value-of select="count(result[starts-with(status, 'Not implemented')])"/>
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
	<P class="chapter" align="center">.:: Detailed Report ::.</P>
	<P align="center">
		<xsl:call-template name="journal" />
	</P>
	<!-- Sort results by prologue / category / name -->
	<xsl:apply-templates select="result[category=$testsuite]">
		<xsl:sort select="name" />
	</xsl:apply-templates>
</xsl:template>

<xsl:template name="journal_sticker">
	<div style="font-size: 8pt; margin-top:0px; top-padding:0px; margin-bottom:2px; margin-right:10px;" align="right">
		<xsl:call-template name="journal" />
	</div>
</xsl:template>

<xsl:template name="journal">
	<b>Journal: </b>
	<a href="javascript:void(0)" onclick="viewLog();" title="View the current journal">View</a>
	&#x007c;
	<a href="javascript:void(0)" onclick="commitLog();" title="Commit changes">Commit</a>
	&#x007c;
	<a href="javascript:void(0)" onclick="clearLog();" title="Clear journal and reload page">Clear</a>
	&#x007c;
	<a href="javascript:void(0)" onclick="downloadZip();" title="Download reference zip archive">&#x21e9;</a>
	&#x007c;
	<a href="javascript:void(0)" onclick="helpLog();" title="Print some help and usage">?</a>
</xsl:template>

<!-- Print result -->
<xsl:template match="result">
	<xsl:variable name="n" select="generate-id(name)"/>
	
	<xsl:choose>
		<xsl:when test="$mode = 0">
			<!-- Only print result if it's part of the current page -->
			<xsl:if test="((position()-1) &gt;= $start) and ((position()-1) &lt;= $end)">
				<xsl:call-template name="journal_sticker" />
				<table align="center" width="1000px" BORDER="0">
					<tr>
						<!-- Test name -->
						<td>
							<xsl:attribute name="width">640px</xsl:attribute>
							<xsl:attribute name="align">left</xsl:attribute>
							<xsl:attribute name="class">testhead</xsl:attribute>
							<B><xsl:value-of select="position()"/></B>: 
							<xsl:if test="string-length(prologue) &gt; 0"><B><xsl:value-of select="prologue"/></B> / </xsl:if>
							<xsl:if test="string-length(category) &gt; 0"><B><xsl:value-of select="category"/></B> / </xsl:if>
							<B><xsl:value-of select="name"/></B>
						</td>
						<!-- Test link -->
						<td>
							<xsl:attribute name="width">96px</xsl:attribute>
							<xsl:attribute name="align">right</xsl:attribute>
							<xsl:attribute name="class">testhead</xsl:attribute>
							<span>
								<xsl:attribute name="class">details</xsl:attribute>
								<xsl:attribute name="id">ai<xsl:value-of select="$n"/></xsl:attribute>
								<A>
									<xsl:attribute name="href"><xsl:value-of select="category"/>_Test<xsl:value-of select="position()"/>.html</xsl:attribute>
									Details
								</A>
							</span>
						</td>
                                                <xsl:choose>
                                                <!-- If the result is a digit value and the test is a failure-->
                                                <xsl:when test="value and starts-with(status, 'Failure')">
						<!-- Actual result -->
						<td>
							<xsl:attribute name="width">256px</xsl:attribute>
							<xsl:attribute name="align">center</xsl:attribute>
							<xsl:attribute name="style">
								<xsl:call-template name="style">
									<xsl:with-param name="status" />
									<xsl:with-param name="diffavg" />
								</xsl:call-template>
							</xsl:attribute>
							<xsl:attribute name="class">testhead</xsl:attribute>
							<xsl:value-of select="value/got"/> (Ref:<xsl:value-of select="value/ref"/>)
						</td>
					</xsl:when>
					<!-- If the result is NOT a digit value or if the test is successful-->
					<xsl:otherwise>
						<!-- Status -->
						<td>
							<xsl:attribute name="width">256px</xsl:attribute>
							<xsl:attribute name="align">center</xsl:attribute>
							<xsl:attribute name="style">
								<xsl:call-template name="style">
									<xsl:with-param name="status" />
									<xsl:with-param name="diffavg" />
								</xsl:call-template>
							</xsl:attribute>
							<xsl:attribute name="class">testhead</xsl:attribute>
							<xsl:call-template name="status">
								<xsl:with-param name="status" />
								<xsl:with-param name="diffavg" />
							</xsl:call-template>
						</td>
					</xsl:otherwise>	
					</xsl:choose>
					</tr>
				</table>
			</xsl:if>
		</xsl:when>
		<xsl:otherwise>
			<!-- Only print result if it's part of the current page -->
			<xsl:if test="position() = $test_name">
			
				<P ALIGN="CENTER">
					<form name="UpdateAllTest" ALIGN="CENTER">
					<input>
						<xsl:attribute name="name">action</xsl:attribute>
						<xsl:attribute name="type">hidden</xsl:attribute>
						<xsl:attribute name="value">update</xsl:attribute>
					</input>
					<input>
						<xsl:attribute name="name">file</xsl:attribute>
						<xsl:attribute name="type">hidden</xsl:attribute>
						<xsl:attribute name="value"><xsl:if test="string-length(prologue) &gt; 0"><xsl:value-of select="prologue"/>/</xsl:if><xsl:if test="string-length(category) &gt; 0"><xsl:value-of select="category"/>/</xsl:if><xsl:value-of select="name"/>/</xsl:attribute>
					</input>
					<select name="revert">
						<option value="0">Update all screenshots</option>
						<option value="1">Revert all screenshots</option>
					</select>
					<br/>
					<a href="javascript:void(0)" onClick="suubmit(document.forms['UpdateAllTest'], '');" style="font-weight:bolder;">Go</a>
					</form>
				</P>

				<xsl:call-template name="journal_sticker" />
				<TABLE align="center" width="1000px" BORDER="0">
					<TR>
						<!-- Test name -->
						<TD>
							<xsl:attribute name="width">640px</xsl:attribute>
							<xsl:attribute name="align">left</xsl:attribute>
							<xsl:attribute name="class">testhead</xsl:attribute>
							<xsl:if test="string-length(prologue) &gt; 0"><B><xsl:value-of select="prologue"/></B> / </xsl:if>
							<xsl:if test="string-length(category) &gt; 0"><B><xsl:value-of select="category"/></B> / </xsl:if>
							<B><xsl:value-of select="name"/></B>
						</TD>
                                               <xsl:choose>
                                                <!-- If the result is a digit value and the test is a failure-->
                                                <xsl:when test="value">
						<!-- Actual result -->
						<td>
							<xsl:attribute name="width">256px</xsl:attribute>
							<xsl:attribute name="align">center</xsl:attribute>
							<xsl:attribute name="style">
								<xsl:call-template name="style">
									<xsl:with-param name="status" />
									<xsl:with-param name="diffavg" />
								</xsl:call-template>
							</xsl:attribute>
							<xsl:attribute name="class">testhead</xsl:attribute>
							<xsl:value-of select="value/got"/> <xsl:if test="starts-with(status, 'Failure')">(Ref:<xsl:value-of select="value/ref"/>)</xsl:if>
						</td>
                                                </xsl:when>
                                                <!-- If the result is NOT a digit value or if the test is successful-->
                                                <xsl:otherwise>
						<!-- Status -->
						<TD>
							<xsl:attribute name="width">256px</xsl:attribute>
							<xsl:attribute name="align">center</xsl:attribute>
							<xsl:attribute name="style">
								<xsl:call-template name="style">
									<xsl:with-param name="status" />
									<xsl:with-param name="diffavg" />
								</xsl:call-template>
							</xsl:attribute>
							<xsl:attribute name="class">testhead</xsl:attribute>
							<xsl:call-template name="status">
								<xsl:with-param name="status" />
								<xsl:with-param name="diffavg" />
							</xsl:call-template>
						</TD>
                                                </xsl:otherwise>	
                                                </xsl:choose>
					</TR>
					<TR>
						<!-- Detail -->
						<TD COLSPAN="2">
							<xsl:attribute name="class">testhead</xsl:attribute>
							<div id="ii{$n}" name="ii{$n}" style="position:relative; ">
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
											<xsl:attribute name="align">center</xsl:attribute>
											<xsl:attribute name="style">font-weight:bolder;</xsl:attribute>
											Mask
										</TD>
										<TD>
											<xsl:attribute name="width">256px</xsl:attribute>
											<xsl:attribute name="align">center</xsl:attribute>
											<xsl:attribute name="style">font-weight:bolder;</xsl:attribute>
											Reference &#x0026; Actual Files
										</TD>
										<TD>
											<xsl:attribute name="align">center</xsl:attribute>
											<xsl:attribute name="style">font-weight:bolder;</xsl:attribute>
											Update
										</TD>
									</TR>
									<!-- Files -->
									<xsl:apply-templates select="file">
										<xsl:sort select="name" />
									</xsl:apply-templates>
								</TABLE>
							</div>
						</TD>
					</TR>
				</TABLE>
                                <xsl:if test="comments">
                                <P class="chapter" align="center">.:: Specification ::.</P>
                                <P ALIGN="CENTER">
                                        <pre>
                                        <xsl:value-of select="comments"/>
                                        </pre>
                                </P>
                                </xsl:if>                                
                                <xsl:if test="file/loggot">
                                <P class="chapter" align="center">.:: Log ::.</P>
                                <P ALIGN="CENTER">
                                <iframe align="center" width="960px" height="480px">
                                <xsl:attribute name="src"><xsl:value-of select="file/loggot"/></xsl:attribute>
                                </iframe>
                                </P>
                                </xsl:if>
			</xsl:if>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>	

<!-- Print statitstics per test suite-->
<xsl:template name="statistics">
	<P class="chapter" align="center">.:: Statistics ::.</P>
	
	<table align="center" width="50%">
		<!-- Table head -->
		<thead bgcolor="#F0F0F0" >
		<tr>
			<th>Test suite</th>
			<th>Run tests</th>
			<th>Passed tests</th>
			<th>Failed tests</th>
			<th>Success rate</th>		
		</tr>
		</thead>
		<!-- Table body -->
		<tbody>
		<xsl:for-each select="statistics/testsuite">
		<xsl:sort select="name" />
		<TR>
			<td><a><xsl:attribute name="href"><xsl:value-of select="name" />.html</xsl:attribute><xsl:value-of select="name"/></a></td>
			<td align="center"><xsl:value-of select="runtests"/></td>
			<td align="center"><xsl:value-of select="passedtests"/></td>
			<td align="center"><xsl:value-of select="failedtests"/></td>
			<xsl:choose>
				<xsl:when test="successrate = 100">
				<td align="center" style="background-color:#22aa22;color:white;"><b><xsl:value-of select="successrate"/></b></td>
				</xsl:when>
				<xsl:otherwise>
				<td align="center" style="background-color:#ff7770;color:white;"><b><xsl:value-of select="successrate"/></b></td>
				</xsl:otherwise>
			</xsl:choose>
		</TR>
		</xsl:for-each>	
		<!-- Table foot -->
		<tr>
			<td><b>Total</b></td>
			<td align="center"><b><xsl:value-of select="statistics/total/runtests"/></b></td>
			<td align="center"><b><xsl:value-of select="statistics/total/passedtests"/></b></td>
			<td align="center"><b><xsl:value-of select="statistics/total/failedtests"/></b></td>
			<xsl:choose>
				<xsl:when test="statistics/total/successrate = 100">
				<td align="center" style="background-color:#22aa22;color:white;"><b><xsl:value-of select="statistics/total/successrate"/></b></td>
				</xsl:when>
				<xsl:otherwise>
				<td align="center" style="background-color:#ff7770;color:white;"><b><xsl:value-of select="statistics/total/successrate"/></b></td>
				</xsl:otherwise>
			</xsl:choose>
		</tr>
		</tbody>
	</table>
</xsl:template>	

<!-- Print file comparison result -->
<xsl:template match="file">
	<xsl:variable name="r" select="ref"/>								<!-- Reference file		-->
	<xsl:variable name="g" select="got"/>							<!-- Actual file			-->
	<xsl:variable name="d" select="diff"/>							<!-- Difference result	-->
	<xsl:variable name="nm" select="name"/>						<!-- File name			-->
	<xsl:variable name="tm" select="generate-id(name)"/>		<!-- File name id		-->
	<xsl:variable name="tn" select="generate-id(../name)"/>	<!-- Test name id		-->
	<xsl:if test="$mode = 1">
		<TR>
			<!-- File index -->
			<TD>
				<xsl:attribute name="width">32px</xsl:attribute>
				<xsl:attribute name="align">center</xsl:attribute>
				<xsl:attribute name="style">
					<xsl:call-template name="style_screen">
						<xsl:with-param name="diffvalue" />
					</xsl:call-template>
				</xsl:attribute>
				<xsl:number value="position()-1" format="1" /><br/>
			</TD>
			
			<xsl:choose>
				<!-- Files are different -->
				<xsl:when test="diffvalue &gt; 0">
					<!-- Difference result -->
					<TD>
						<xsl:attribute name="align">center</xsl:attribute>
						<xsl:attribute name="valign">center</xsl:attribute>
						<xsl:choose>
							<xsl:when test="string-length(diff) &gt; 0">
								<xsl:choose>
									<xsl:when test="type = 'image'">
										<a href="{$d}"><img src="{$d}" title="Result mask" border="0" style="display:block"/></a>
									</xsl:when>
									<xsl:otherwise>
										<a href="{$d}">&#x21e9; Diff file</a><br/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								Cannot find diff file for <b><xsl:value-of select="name"/></b>.
							</xsl:otherwise>
						</xsl:choose>
					</TD>
					<!-- Reference and actual files -->
					<TD>
						<xsl:attribute name="align">center</xsl:attribute>
						<xsl:attribute name="valign">center</xsl:attribute>
						<!-- Reference -->
						<xsl:choose>
							<xsl:when test="string-length(ref) &gt; 0">
								<xsl:choose>
									<xsl:when test="type = 'image'">
										<a href="{$r}"><img src="{$r}" width="256px" title="Reference screenshot" border="0" style="display:block" /></a><br/>
									</xsl:when>
									<xsl:otherwise>
										<a href="{$r}">&#x21e9; Reference</a><br/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								No reference file<br/>
							</xsl:otherwise>
						</xsl:choose>
						<!-- Difference in %age -->
						<b>Diff: <xsl:value-of select="diffvalue"/>%</b><br/>
						
						<!-- Actual file -->
						<xsl:choose>
							<xsl:when test="string-length(got) &gt; 0">
								<xsl:choose>
									<xsl:when test="type = 'image'">
										<br/><a href="{$g}"><img src="{$g}" width="256px" title="Actual screenshot" border="0" style="display:block" /></a>
									</xsl:when>
									<xsl:otherwise>
										<a href="{$g}">&#x21e9; Actual</a><br/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								File has disappeared
							</xsl:otherwise>
						</xsl:choose>
					</TD>
				</xsl:when>
				<!-- Identical files -->
				<xsl:otherwise>
					<!-- File -->
					<TD>
						<xsl:attribute name="align">center</xsl:attribute>
						<xsl:attribute name="valign">center</xsl:attribute>
						<xsl:if test="string-length(got) &gt; 0">
							<xsl:choose>
								<xsl:when test="type = 'image'">
									<a href="{$g}"><img src="{$g}" title="Actual screenshot" border="0" style="display:block" /></a>
								</xsl:when>
								<xsl:otherwise>
									<a href="{$g}">&#x21e9; Actual file</a><br/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:if>
					</TD>
					<!-- Status -->
					<TD>
						<xsl:attribute name="align">center</xsl:attribute>
						<xsl:attribute name="valign">center</xsl:attribute>
						<b>Perfect match &#x2713;</b>
					</TD>
				</xsl:otherwise>
			</xsl:choose>
			
			<TD>
				<xsl:attribute name="align">center</xsl:attribute>
				
				<form name="f{$tm}">
					<!--xsl:attribute name="method">GET</xsl:attribute-->
					<!--xsl:attribute name="target">blank</xsl:attribute-->
					<!--xsl:attribute name="action">/autobuilds/refmgr.cgi</xsl:attribute-->
					<input>
						<xsl:attribute name="name">action</xsl:attribute>
						<xsl:attribute name="type">hidden</xsl:attribute>
						<xsl:attribute name="value">update</xsl:attribute>
					</input>
					<input>
						<xsl:attribute name="name">file</xsl:attribute>
						<xsl:attribute name="type">hidden</xsl:attribute>
						<xsl:attribute name="value"><xsl:value-of select="name"/></xsl:attribute>
					</input>
					<select name="revert">
						<option value="0">Update</option>
						<option value="1">Revert</option>
					</select>
					<br/><a href="javascript:void(0)" onClick="suubmit(document.forms['f{$tm}'], '{$tm}');" style="font-weight:bolder;">Go</a>
				</form>
				<br/>
				<img name="{$tm}">
					<xsl:attribute name="src">/autobuilds/refmgr.cgi?action=image&amp;file=<xsl:value-of select="name"/></xsl:attribute>
				</img>
			</TD>
		</TR>
	</xsl:if>
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
				<xsl:when test="diffavg &gt; $sucess_threshold">
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
		<xsl:when test="starts-with(status, 'Not implemented')">
			font-weight:bolder; background-color:#AAAAAA; color:white;
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
				<xsl:when test="diffavg &gt; $sucess_threshold">
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
