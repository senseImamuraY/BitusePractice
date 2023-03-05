//グローバル
cbuffer global
{
	matrix g_WVP; //ワールドから射影までの変換行列
};

//
//
//バーテックスシェーダー
float4 VSMain( float4 Pos : POSITION ) : SV_POSITION
{
	Pos=mul(Pos,g_WVP);
	return Pos;
}

//
//
//ピクセルシェーダー
float4 PSMain( float4 Pos : SV_POSITION ) : SV_Target
{
	return float4(1,1,1,1);
}