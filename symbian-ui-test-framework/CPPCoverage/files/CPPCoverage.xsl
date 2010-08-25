<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:param name="mode" />
<xsl:param name="module" />
<xsl:param name="source" />
<xsl:param name="version" />

<!-- Main entry point -->
<xsl:template match="/">
	<xsl:choose>
		<xsl:when test="$mode = 'menu'">
			<!-- Print the menu (ie, modules names) -->
			<table class="mtable" align="center" width="100%">
				<tr>
					<td valign="center">
						<strong>Select a module: </strong>
						<xsl:for-each select="codecoverage/modules/module">
							<xsl:sort select="@id"/>
							&#x0020;[<a>
									<xsl:attribute name="href">#</xsl:attribute>
									<xsl:attribute name="onclick">showModuleStats('<xsl:value-of select="@id"/>')</xsl:attribute>
									<xsl:value-of select="@id"/>
							</a>]
						</xsl:for-each>
					</td>
					<td align="right" valign="center"><img src="home.gif" title="Main menu" style="cursor:hand" onclick="home()" />&#x0020;&#x0020;&#x0020;<strong>v<xsl:value-of select="$version"/></strong></td>
				</tr>
			</table>
		</xsl:when>
		<xsl:when test="$mode = 'module'">
			<!-- Print the module's stats -->
			<h1 align="center">Module statistics for <xsl:value-of select="$module"/>:</h1>
			<xsl:for-each select="codecoverage/modules/module">
				<xsl:if test="@id = $module">
					<!-- stats -->
					<table class="modsummary" align="center">
						<tr>
							<td align="right"><strong>Lines:</strong></td><td align="right" width="300px"><xsl:value-of select="linecount"/></td>
						</tr>
						<tr>
							<td align="right"><strong>Coverage:</strong></td><td align="right" width="300px">
								<xsl:variable name="modncovdcount" select="count(/codecoverage/files/file[/codecoverage/modules/module[@id = $module]/reflist/fileref/@id = @id]/lines/line[cov = 'n'])"/>
								<xsl:variable name="modcovdcount" select="count(/codecoverage/files/file[/codecoverage/modules/module[@id = $module]/reflist/fileref/@id = @id]/lines/line[cov = 'y'])"/>
								<xsl:variable name="modcoverage" select="round(100 * ($modcovdcount div ($modcovdcount + $modncovdcount)))"/>
								<xsl:value-of select="$modcoverage"/>%
							</td>
						</tr>
					</table>
		
					<h3 align="center">Legend</h3>
					<TABLE class="mtable" align="center" width="80%">
						<TR>
							<TD class="covn" align="center" width="100px">28%</TD>
							<TD>Red cells indicate a coverage below 50%. This means that more tests should be written.</TD>
						</TR>
						<TR>
							<TD class="covy" align="center" width="100px">93%</TD>
							<TD>Green cells indicate a coverage above 80%, which is good, although more tests could be written in order to reach the Holy Grail (means 100%).</TD>
						</TR>
						<TR>
							<TD width="100px" align="center">68%</TD>
							<TD>Plain cells indicate an average coverage (which is still a bad thing, as everything should be in green).</TD>
						</TR>
					</TABLE>
					<BR/>

					<!-- source files -->
					<h3 align="center">Source files</h3>
					<table class="modsummary" align="center">
						<tr>
							<th>File name</th>
							<th>Audited</th>
							<th>Covered</th>
						</tr>
						<xsl:for-each select="reflist/fileref">
							<xsl:sort select="@id"/>
							<xsl:variable name="refid" select="@id"/>
							<xsl:variable name="filecount"		select="/codecoverage/files/file[@id = $refid]/linecount"/>
							<xsl:variable name="filecovered"	select="count(/codecoverage/files/file[@id = $refid]/lines/line[cov = 'y'])"/>
							<xsl:variable name="filenotcovered"	select="count(/codecoverage/files/file[@id = $refid]/lines/line[cov = 'n'])"/>
							<xsl:variable name="fileaudited"	select="round((($filecovered + $filenotcovered) div $filecount) * 100)"/>
							<xsl:variable name="filecoverage"	select="round(($filecovered div ($filecovered + $filenotcovered)) * 100)"/>
							<tr>
								<td align="left">
									<a>
										<xsl:attribute name="href">#</xsl:attribute>
										<xsl:attribute name="onclick">showFileStats('<xsl:value-of select="$module"/>', '<xsl:value-of select="@id"/>')</xsl:attribute>
										<xsl:value-of select="@id"/>
									</a>
								</td>
								<td align="right">
									<xsl:choose>
										<xsl:when test="$fileaudited &gt; 0">
											<xsl:value-of select="$fileaudited"/>%
										</xsl:when>
										<xsl:otherwise>
											0%
										</xsl:otherwise>
									</xsl:choose>
								</td>
								
								<xsl:choose>
									<xsl:when test="$filecoverage &lt; 50">
										<td align="right" class="covn"><xsl:value-of select="$filecoverage"/>%</td>
									</xsl:when>
									<xsl:when test="$filecoverage &gt; 80">
										<td align="right" class="covy"><xsl:value-of select="$filecoverage"/>%</td>
									</xsl:when>
									<xsl:otherwise>
										<td align="right"><xsl:value-of select="$filecoverage"/>%</td>
									</xsl:otherwise>
								</xsl:choose>
							</tr>
						</xsl:for-each>
					</table>
				</xsl:if>
			</xsl:for-each>
		</xsl:when>
		<xsl:when test="$mode = 'source'">
			<!-- Print the file's stats -->
			<h1 align="center">Module statistics for <xsl:value-of select="$module"/>:</h1>
			<xsl:variable name="srccount"		select="/codecoverage/files/file[@id = $source]/linecount"/>
			<xsl:variable name="srccovered"		select="count(/codecoverage/files/file[@id = $source]/lines/line[cov = 'y'])"/>
			<xsl:variable name="srcnotcovered"	select="count(/codecoverage/files/file[@id = $source]/lines/line[cov = 'n'])"/>
			<xsl:variable name="srcaudited"		select="round((($srccovered + $srcnotcovered) div $srccount) * 100)"/>
			<xsl:variable name="srccoverage"	select="round(($srccovered div ($srccovered + $srcnotcovered)) * 100)"/>
			<table class="modsummary" align="center">
				<tr>
					<th>File name</th>
					<th>Audited</th>
					<th>Covered</th>
				</tr>
				<tr>
					<td align="right">
						<xsl:value-of select="$source"/>
					</td>
					<td align="right">
						<xsl:value-of select="$srcaudited"/>%
					</td>
					<xsl:choose>
						<xsl:when test="$srccoverage &lt; 50">
							<td align="right" class="covn"><xsl:value-of select="$srccoverage"/>%</td>
						</xsl:when>
						<xsl:when test="$srccoverage &gt; 80">
							<td align="right" class="covy"><xsl:value-of select="$srccoverage"/>%</td>
						</xsl:when>
						<xsl:otherwise>
							<td align="right"><xsl:value-of select="$srccoverage"/>%</td>
						</xsl:otherwise>
					</xsl:choose>
				</tr>
			</table>
			<h3 align="center">Legend</h3>
			<TABLE class="mtable" align="center" width="80%">
				<TR>
					<TD class="covn" align="center" width="100px">&#x0020;</TD>
					<TD>Red cells indicate a line the program never encountered.</TD>
				</TR>
				<TR>
					<TD class="covy" align="center" width="100px">&#x0020;</TD>
					<TD>Green cells indicate code that has been tested.</TD>
				</TR>
			</TABLE>
			<BR/>
			
			<h3 align="center">Audited lines:</h3>
			<table class="modsummary" style="border-collapse:collapse" align="center" width="80%">
				<tr>
					<th>#</th>
					<th>Line</th>
				</tr>
				<xsl:for-each select="/codecoverage/files/file[@id = $source]/lines/line">
					<xsl:sort select="ord" data-type="number"/>
					<tr>
						<td width="80px" align="right"><strong><xsl:value-of select="ord"/></strong></td>
						<td>
						<xsl:attribute name="class">cov<xsl:value-of select="cov"/></xsl:attribute>
						<input readonly="">
							<xsl:attribute name="type">text</xsl:attribute>
							<xsl:attribute name="class">cov<xsl:value-of select="cov"/></xsl:attribute>
							<xsl:attribute name="style">width:100%; border-top:0px solid none; border-left:0px solid none; border-right:0px solid none; border-bottom:1px dotted black;</xsl:attribute>
							<xsl:attribute name="value"><xsl:value-of select="src"/></xsl:attribute>
						</input>
						</td>
					</tr>
				</xsl:for-each>
			</table>
			<HR/>
		</xsl:when>
		<xsl:otherwise>
			<strong>Error: unknown parameter <xsl:value-of select="$mode"/></strong><br/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="codecoverage">
	<xsl:apply-templates select="modules" />
</xsl:template>

<xsl:template match="modules">
	<xsl:apply-templates select="module">
		<xsl:sort select="name"/>
	</xsl:apply-templates>
</xsl:template>

<xsl:template match="module">
	<li><xsl:value-of select="name"/></li>
</xsl:template>

</xsl:stylesheet>
