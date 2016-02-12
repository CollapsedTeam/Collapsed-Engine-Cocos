<GameFile>
  <PropertyGroup Name="scene" Type="Scene" ID="a2ee0952-26b5-49ae-8bf9-4f1d6279b798" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" ctype="GameNodeObjectData">
        <Size X="1280.0000" Y="720.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="-658410969" Tag="3" IconVisible="False" ctype="SpriteObjectData">
            <Size X="1280.0000" Y="720.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="Shared/background.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="lblLoading" ActionTag="1284091834" Tag="5" IconVisible="False" TopMargin="586.0000" BottomMargin="94.0000" IsCustomSize="True" FontSize="32" LabelText="L O A D I N G . . ." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="TextObjectData">
            <Size X="1280.0000" Y="40.0000" />
            <AnchorPoint />
            <Position Y="94.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="0.1306" />
            <PreSize X="1.0000" Y="0.0556" />
            <FontResource Type="Normal" Path="Shared/font.ttf" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="255" G="127" B="80" />
          </AbstractNodeData>
          <AbstractNodeData Name="loadingBar" ActionTag="-2062367673" Tag="3" IconVisible="False" LeftMargin="100.0000" RightMargin="100.0000" TopMargin="648.0000" BottomMargin="56.0000" ProgressInfo="100" ctype="LoadingBarObjectData">
            <Size X="1080.0000" Y="16.0000" />
            <AnchorPoint />
            <Position X="100.0000" Y="56.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0781" Y="0.0778" />
            <PreSize X="0.8438" Y="0.0222" />
            <ImageFileData Type="PlistSubImage" Path="loading_bar.png" Plist="Screens/Loading/skin.plist" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>