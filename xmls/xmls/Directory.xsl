<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:template match="/">
	<html>
	<body>
<xsl:apply-templates/>
	</body>
	</html>
</xsl:template>

<xsl:template match="Directory" name="Directory">
	<xsl:choose>
		<xsl:if test="@path">
			<b><font color="#0000ff"><xsl:value-of select="@path" /></font></b><br/>
		</xsl:if>
		<xsl:otherwise>
			<li>
			<b><font color="#0000ff"><xsl:value-of select="@name" /></font></b> <small> <i> (
			<xsl:value-of select="count(child::*[@name])"/> items) </i> </small>
			</li>
		</xsl:otherwise>
	</xsl:choose>
	<ul>
	<xsl:for-each select="Directory">
		<xsl:sort select="@name" />
		<xsl:call-template name="Directory" />
	</xsl:for-each>
	<xsl:for-each select="File">
		<xsl:sort select="@name" />
		<xsl:call-template name="File" />
	</xsl:for-each>
	<xsl:for-each select="Link">
		<xsl:sort select="@name" />
		<xsl:call-template name="Link" />
	</xsl:for-each>
	<xsl:for-each select="Fifo">
		<xsl:sort select="@name" />
		<xsl:call-template name="Fifo" />
	</xsl:for-each>
	<xsl:for-each select="Socket">
		<xsl:sort select="@name" />
		<xsl:call-template name="Socket" />
	</xsl:for-each>
	<xsl:for-each select="CharacterDevice">
		<xsl:sort select="@name" />
		<xsl:call-template name="CharacterDevice" />
	</xsl:for-each>
	<xsl:for-each select="BlockDevice">
		<xsl:sort select="@name" />
		<xsl:call-template name="BlockDevice" />
	</xsl:for-each>
	</ul>
</xsl:template>

<xsl:template match="File" name="File">
	<li>
	<i><xsl:value-of select="@name" /></i> <small> <i> (
	<xsl:value-of select="@size" /> bytes) </i></small>
	<xsl:if test="@type">
		__________ <font size="-2"><xsl:value-of select="@type" /></font>
	</xsl:if>
	</li>
</xsl:template>

<xsl:template match="Link" name="Link">
	<li>
	@<i><font color="#00EEEE"><xsl:value-of select="@name" /></font></i>
	__________ <font size="-2">Symbolic Link</font>
	</li>
</xsl:template>

<xsl:template match="Socket" name="Socket">
	<li>
	&amp;<i><font color="EEEE00"><xsl:value-of select="@name" /></font></i>
	__________ <font size="-2">Socket</font>
	</li>
</xsl:template>

<xsl:template match="BlockDevice" name="BlockDevice">
	<li>
	<i><font color="B08060"><xsl:value-of select="@name" /></font></i>
	__________ <font size="-2">Block Device</font>
	</li>
</xsl:template>

<xsl:template match="CharacterDevice" name="CharacterDevice">
	<li>
	<i><font color="B08060"><xsl:value-of select="@name" /></font></i>
	__________ <font size="-2">Character Device</font>
	</li>
</xsl:template>

<xsl:template match="Fifo" name="Fifo">
	<li>
	<i><xsl:value-of select="@name" />|</i>
	__________ <font size="-2">Named Pipe</font>
	</li>
</xsl:template>

</xsl:stylesheet>

