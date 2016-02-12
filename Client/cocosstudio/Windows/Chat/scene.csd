<GameFile>
  <PropertyGroup Name="scene" Type="Scene" ID="d10a1849-8e04-488f-b3d7-1a0e9badeefa" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" ctype="GameNodeObjectData">
        <Size X="1280.0000" Y="720.0000" />
        <Children>
          <AbstractNodeData Name="Chat" ActionTag="-794032476" Tag="13" IconVisible="False" RightMargin="847.0000" TopMargin="443.0000" Scale9Enable="True" LeftEage="142" RightEage="142" TopEage="91" BottomEage="91" Scale9OriginX="142" Scale9OriginY="91" Scale9Width="149" Scale9Height="95" ctype="ImageViewObjectData">
            <Size X="433.0000" Y="277.0000" />
            <Children>
              <AbstractNodeData Name="btnChannel0" ActionTag="-2017111983" Tag="43" IconVisible="False" TopMargin="0.0001" BottomMargin="243.9999" TouchEnable="True" FontSize="18" ButtonText="[channel]" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="10" BottomEage="10" Scale9OriginX="15" Scale9OriginY="10" Scale9Width="52" Scale9Height="13" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                <Size X="433.0000" Y="33.0000" />
                <AnchorPoint />
                <Position Y="243.9999" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.8809" />
                <PreSize X="1.0000" Y="0.1191" />
                <FontResource Type="Normal" Path="Shared/font.ttf" Plist="" />
                <TextColor A="255" R="205" G="205" B="205" />
                <DisabledFileData Type="PlistSubImage" Path="normal_button_d.png" Plist="Windows/Chat/skin.plist" />
                <PressedFileData Type="PlistSubImage" Path="normal_button_c.png" Plist="Windows/Chat/skin.plist" />
                <NormalFileData Type="PlistSubImage" Path="normal_button_n.png" Plist="Windows/Chat/skin.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lstChat" ActionTag="1139727289" Tag="42" IconVisible="False" LeftMargin="2.0000" RightMargin="1.0000" TopMargin="34.0000" BottomMargin="33.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
                <Size X="430.0000" Y="210.0000" />
                <AnchorPoint />
                <Position X="2.0000" Y="33.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0046" Y="0.1191" />
                <PreSize X="0.9931" Y="0.7581" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="btnSend" ActionTag="-455687642" Tag="4" IconVisible="False" LeftMargin="339.0000" TopMargin="244.0000" TouchEnable="True" FontSize="18" ButtonText="Send" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="10" BottomEage="10" Scale9OriginX="15" Scale9OriginY="10" Scale9Width="64" Scale9Height="13" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
                <Size X="94.0000" Y="33.0000" />
                <AnchorPoint ScaleX="1.0000" ScaleY="1.0000" />
                <Position X="433.0000" Y="33.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="0.1191" />
                <PreSize X="0.2171" Y="0.1191" />
                <FontResource Type="Normal" Path="Shared/font.ttf" Plist="" />
                <TextColor A="255" R="205" G="205" B="205" />
                <DisabledFileData Type="PlistSubImage" Path="rounded_rb_button_d.png" Plist="Windows/Chat/skin.plist" />
                <PressedFileData Type="PlistSubImage" Path="rounded_rb_button_c.png" Plist="Windows/Chat/skin.plist" />
                <NormalFileData Type="PlistSubImage" Path="rounded_rb_button_n.png" Plist="Windows/Chat/skin.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="txtChatBg" ActionTag="2130140425" Tag="10" IconVisible="False" RightMargin="93.0000" TopMargin="244.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" LeftEage="112" RightEage="112" TopEage="10" BottomEage="10" Scale9OriginX="112" Scale9OriginY="10" Scale9Width="116" Scale9Height="13" ScrollDirectionType="Horizontal" ctype="ScrollViewObjectData">
                <Size X="340.0000" Y="33.0000" />
                <AnchorPoint />
                <Position />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition />
                <PreSize X="0.7852" Y="0.1191" />
                <FileData Type="PlistSubImage" Path="chat_textbox.png" Plist="Windows/Chat/skin.plist" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="512" Height="33" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="216.5000" Y="138.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1691" Y="0.1924" />
            <PreSize X="0.3383" Y="0.3847" />
            <FileData Type="PlistSubImage" Path="chat_bg.png" Plist="Windows/Chat/skin.plist" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>