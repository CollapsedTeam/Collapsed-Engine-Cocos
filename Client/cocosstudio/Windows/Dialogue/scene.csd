<GameFile>
  <PropertyGroup Name="screen" Type="Scene" ID="e6e15022-e1bc-42da-a411-d1fcc8cd6065" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="24" ctype="GameNodeObjectData">
        <Size X="1280.0000" Y="720.0000" />
        <Children>
          <AbstractNodeData Name="Dialogue" ActionTag="742946650" Tag="9" IconVisible="False" LeftMargin="16.0000" RightMargin="462.0000" TopMargin="488.0000" BottomMargin="8.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="802" Scale9Height="224" ctype="PanelObjectData">
            <Size X="802.0000" Y="224.0000" />
            <Children>
              <AbstractNodeData Name="lblWho" ActionTag="1926290086" ZOrder="1" Tag="77" IconVisible="False" LeftMargin="82.5000" RightMargin="584.5000" TopMargin="3.0000" BottomMargin="190.0000" TouchEnable="True" FontSize="22" LabelText="Who" HorizontalAlignmentType="HT_Center" OutlineEnabled="True" ShadowOffsetX="1.0000" ShadowOffsetY="1.0000" ctype="TextObjectData">
                <Size X="47.0000" Y="31.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="150.0000" Y="190.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="205" G="205" B="205" />
                <PrePosition X="0.1870" Y="0.8482" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="Shared/font.ttf" Plist="" />
                <OutlineColor A="255" R="47" G="47" B="47" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblText" ActionTag="-542852314" ZOrder="1" Tag="10" IconVisible="False" LeftMargin="8.0000" RightMargin="8.0000" TopMargin="42.0000" BottomMargin="16.0000" TouchEnable="True" IsCustomSize="True" FontSize="22" LabelText="Text&#xA;&#xA;" OutlineEnabled="True" ShadowOffsetX="1.0000" ShadowOffsetY="1.0000" ctype="TextObjectData">
                <Size X="786.0000" Y="166.0000" />
                <AnchorPoint />
                <Position X="8.0000" Y="16.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="205" G="205" B="205" />
                <PrePosition X="0.0100" Y="0.0714" />
                <PreSize X="0.9800" Y="0.7411" />
                <FontResource Type="Normal" Path="Shared/font.ttf" Plist="" />
                <OutlineColor A="255" R="47" G="47" B="47" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="16.0000" Y="8.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0125" Y="0.0111" />
            <PreSize X="0.6266" Y="0.3111" />
            <FileData Type="PlistSubImage" Path="dialogue_body.png" Plist="Windows/Dialogue/skin.plist" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Options" ActionTag="1905295369" Tag="10" IconVisible="False" LeftMargin="828.0000" RightMargin="16.0000" TopMargin="512.0000" BottomMargin="8.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="480" Scale9Height="200" ctype="PanelObjectData">
            <Size X="436.0000" Y="200.0000" />
            <Children>
              <AbstractNodeData Name="lstOptions" ActionTag="1749034889" Tag="11" IconVisible="False" LeftMargin="8.0000" RightMargin="8.0000" TopMargin="10.0000" BottomMargin="8.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
                <Size X="420.0000" Y="182.0000" />
                <Children>
                  <AbstractNodeData Name="lblOption" ActionTag="715618458" Tag="12" IconVisible="False" LeftMargin="-5.0000" RightMargin="5.0000" BottomMargin="142.0000" TouchEnable="True" IsCustomSize="True" FontSize="22" LabelText="1) Answer&#xA;&#xA;" VerticalAlignmentType="VT_Center" OutlineEnabled="True" ShadowOffsetX="1.0000" ShadowOffsetY="1.0000" ctype="TextObjectData">
                    <Size X="420.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="210.0000" Y="142.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="205" G="205" B="205" />
                    <PrePosition X="0.1658" Y="0.8297" />
                    <PreSize X="1.0000" Y="0.2198" />
                    <FontResource Type="Normal" Path="Shared/font.ttf" Plist="" />
                    <OutlineColor A="255" R="47" G="47" B="47" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="8.0000" Y="8.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0183" Y="0.0400" />
                <PreSize X="0.9633" Y="0.9100" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="828.0000" Y="8.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.6469" Y="0.0111" />
            <PreSize X="0.3406" Y="0.2778" />
            <FileData Type="PlistSubImage" Path="dialogue_options.png" Plist="Windows/Dialogue/skin.plist" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>