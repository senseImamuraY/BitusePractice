//�O���[�o��
cbuffer global
{
	matrix g_WVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
float4 VSMain( float4 Pos : POSITION ) : SV_POSITION
{
	Pos=mul(Pos,g_WVP);
	return Pos;
}

//
//
//�s�N�Z���V�F�[�_�[
float4 PSMain( float4 Pos : SV_POSITION ) : SV_Target
{
	return float4(1,1,1,1);
}