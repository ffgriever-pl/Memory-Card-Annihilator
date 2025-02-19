#include "Include\GUIFrameTexturePS2.h"


CGUIFrameTexturePS2::CGUIFrameTexturePS2(void)
	: m_texbuf(NULL)
	, m_texbuf8(NULL)
	, m_loadedVram(false)
{
	//m_Texture.Filter = GS_FILTER_NEAREST;
	m_Texture.Filter = GS_FILTER_LINEAR;
	//printf("Texture: this = %08x, list loc = %08x\n", this, &m_tex_list);
	m_tex_list.push_back(this);
	m_loadedVram = false;
	//printf("Texture po: this = %08x, list loc = %08x\n", this, &m_tex_list);
	//setRadius(TX_GAUSSIAN_RADIUS);
}


CGUIFrameTexturePS2::~CGUIFrameTexturePS2(void)
{
	std::list<CGUIFrameTexturePS2*>::iterator k;
	for (k = m_tex_list.begin(); k != m_tex_list.end();)
	{
		if ( (*k) == this)
			k = m_tex_list.erase(k);
		else
			k++;
	}

	if (m_texbuf)
	{
		delete [] m_texbuf;
		m_texbuf = NULL;
		m_texbuf8 = NULL;
	}
}

CGUIFrameTexturePS2::eTextureResult CGUIFrameTexturePS2::loadTextureBufferRaw(int PSM, u8 *buffer, int width, int height, int size)
{
	if (m_bTexLoaded) return etResFail;

	u32 size128 = ((size+15)>>4);
	m_texbuf = new u128[size128];
	m_texbuf8 = (u8*) m_texbuf;
	memcpy(m_texbuf8, buffer, size);

	m_Texture.Width = width;
	m_Texture.Height = height;
	m_Texture.PSM = PSM;
	m_Texture.Mem = (u32*)m_texbuf;

	m_width = m_Texture.Width;
	m_height = m_Texture.Height;
	switch (PSM)
	{
		case GS_PSM_CT32:
			m_bpp = 32;
			break;
		case GS_PSM_CT24:
			m_bpp = 24;
			break;
		case GS_PSM_CT16:
		case GS_PSM_CT16S:
			m_bpp = 16;
			break;
		case GS_PSM_T8:
			m_bpp = 8;
			break;
		default:
			m_bpp = 4;
			break;
	}

	m_bTexLoaded = true;
	return etResOk;
}

CGUIFrameTexturePS2::eTextureResult CGUIFrameTexturePS2::loadTextureBuffer(u8 *buffer, u32 size, bool staticBuffer)
{
	if (m_bTexLoaded) return etResFail;
	if (!staticBuffer)
	{
		u32 size128 = ((size+15)>>4);
		m_texbuf = new u128[size128];
		m_texbuf8 = (u8*) m_texbuf;
		memcpy(m_texbuf8, buffer, size);
		if (!loadTextureBufferTim2(m_texbuf8))
		{
			delete [] m_texbuf;
			m_texbuf = NULL;
			m_texbuf8 = NULL;
			return etResNotSupported; //invalid tim2 file
		}
	} else
	{
		if (!loadTextureBufferTim2(buffer)) return etResNotSupported; //invalid tim2 file
	}

	m_bTexLoaded = true;
	return etResOk;
}

CGUIFrameTexturePS2::eTextureResult CGUIFrameTexturePS2::loadTextureFile(char filename[])
{
	//FIXME: Add support
	return etResFuncUnsupported;
}

void CGUIFrameTexturePS2::setTexfilter(eTextureFilter filter)
{
	if (filter == etFiltNearest) m_Texture.Filter = GS_FILTER_NEAREST;
	else m_Texture.Filter = GS_FILTER_LINEAR;
}

bool CGUIFrameTexturePS2::getVramStatus()
{
	return m_loadedVram;
}

void CGUIFrameTexturePS2::setVramStatus(bool status)
{
	m_loadedVram = status;
}

bool CGUIFrameTexturePS2::loadTextureBufferTim2(u8 *res_data)
{
	t_tim2header *tempTim2Head = tempTim2Head = (t_tim2header *)res_data;

	if (tempTim2Head->magic != 0x324d4954 && tempTim2Head->magic != 0x334d4954) return false; //TIM2 & TIM3 headers
	if (tempTim2Head->mangled == 6) return false; //mangled images not supported

	m_Texture.Width = tempTim2Head->width;
	m_Texture.Height = tempTim2Head->height;

	switch (tempTim2Head->bpp)
	{
		case 1: //16bpp
			m_Texture.PSM = GS_PSM_CT16;
			m_bpp = 16;
			break;
		case 2: //24bpp
			m_Texture.PSM = GS_PSM_CT24;
			m_bpp = 24;
			break;
		case 3: //32bpp
			m_Texture.PSM = GS_PSM_CT32;
			m_bpp = 32;
			break;
		case 4: //4bpp
			m_Texture.PSM = GS_PSM_T4;
			m_bpp = 4;
			break;
		default: //8bpp
			m_Texture.PSM = GS_PSM_T8;
			m_bpp = 8;
			break;
	}

	if (m_Texture.PSM == GS_PSM_T4 || m_Texture.PSM == GS_PSM_T8)
	{
		//FIXME: Add support for 16bit clut
		m_Texture.ClutPSM = GS_PSM_CT32;
		if (m_Texture.PSM == GS_PSM_T4)
		{
			m_Texture.Width = (m_Texture.Width+1)&0xfffffffe;
		}
		m_Texture.Clut = (u32*)&res_data[(((tempTim2Head->headerLen + 0x10 + (tempTim2Head->width*tempTim2Head->height / (tempTim2Head->bpp == 5 ? 1 : 2))))+0xf)&0xfffffff0];
	} else
	{
		m_Texture.ClutPSM = 0;
	}
	m_width = m_Texture.Width;
	m_height = m_Texture.Height;
	m_Texture.Mem = (u32*)&res_data[tempTim2Head->headerLen+0x10];
	//printf("W: %d, H: %d, CL: 0x%08x, MEM: 0x%08x\n", texture->Width, texture->Height, (tempTim2Head->headerLen + 0x10 + (tempTim2Head->width*tempTim2Head->height / (tempTim2Head->bpp == 5 ? 1 : 2))),tempTim2Head->headerLen+0x10);
	return true;
}

GSTEXTURE *CGUIFrameTexturePS2::getTexPointer()
{
	return &m_Texture;
}

void CGUIFrameTexturePS2::blur(int size)
{
	if (!m_bTexLoaded) return;
	switch (m_bpp)
	{
		case 24:
			//setRadius(TX_GAUSSIAN_RADIUS);
			//blur24st((u8*)m_Texture.Mem, 0, 0, m_Texture.Width, m_Texture.Height, m_Texture.Width, m_Texture.Height);
			//fastblur24((u8*)m_Texture.Mem, 2, m_Texture.Width, m_Texture.Height);
			fastblur24((u8*)m_Texture.Mem, 1, m_Texture.Width, m_Texture.Height);
			//fastblur24((u8*)m_Texture.Mem, 1, m_Texture.Width, m_Texture.Height);
			m_loadedVram = false;
			break;
		default:
			//not supported
			break;
	}
}

void CGUIFrameTexturePS2::setRadius(int sz)
{

	int i,j;
	if (radius==sz) return;
	kernelSize=1+sz*2;
	radius=sz;

	int sum=0;
	for (i=1;i<sz;i++)
	{
		int szi=sz-i;
		kernel[sz+i]=kernel[szi]=szi*szi;
		sum+=kernel[szi]+kernel[szi];
		for (j=0;j<256;j++)
		{
			mult[sz+i][j]=mult[szi][j]=kernel[szi]*j;
		}
	}
	kernel[sz]=sz*sz;
	sum+=kernel[sz];
	for (j=0;j<256;j++)
	{
		mult[sz][j]=kernel[sz]*j;
	}
}

void CGUIFrameTexturePS2::blur24(u8 *pix, int x, int y,int w,int h, int imw, int imh)
{

	int sum,cr,cg,cb;
	int read,i,ri,xl,yl,yi,ym,riw;

	int iw=imw;

	int wh=iw*imh;

	u8 *r=new u8[wh];
	u8 *g=new u8[wh];
	u8 *b=new u8[wh];

	for (i=0;i<wh;i++)
	{
		r[i]=pix[i*3 +0];
		g[i]=pix[i*3 +1];
		b[i]=pix[i*3 +2];
	}

	u8 *r2=new u8[wh];
	u8 *g2=new u8[wh];
	u8 *b2=new u8[wh];

	x=txmax(0,x);
	y=txmax(0,y);
	w=x+w-txmax(0,(x+w)-iw);
	h=y+h-txmax(0,(y+h)-imh);
	yi=y*iw;

	for (yl=y;yl<h;yl++)
	{
		for (xl=x;xl<w;xl++)
		{
			cb=cg=cr=sum=0;
			ri=xl-radius;
			for (i=0;i<kernelSize;i++)
			{
				read=ri+i;
				if (read>=x && read<w)
				{
					read+=yi;
					cr+=mult[i][r[read]];
					cg+=mult[i][g[read]];
					cb+=mult[i][b[read]];
					sum+=kernel[i];
				}
			}
			ri=yi+xl;
			r2[ri]=cr/sum;
			g2[ri]=cg/sum;
			b2[ri]=cb/sum;
		}
		yi+=iw;
	}
	yi=y*iw;

	for (yl=y;yl<h;yl++)
	{
		ym=yl-radius;
		riw=ym*iw;
		for (xl=x;xl<w;xl++)
		{
			cb=cg=cr=sum=0;
			ri=ym;
			read=xl+riw;
			for (i=0;i<kernelSize;i++)
			{
				if (ri<h && ri>=y)
				{
					cr+=mult[i][r2[read]];
					cg+=mult[i][g2[read]];
					cb+=mult[i][b2[read]];
					sum+=kernel[i];
				}
				ri++;
				read+=iw;
			}
			pix[(xl+yi)*3 +0]= cr/sum;
			pix[(xl+yi)*3 +1]= cg/sum;
			pix[(xl+yi)*3 +2]= cb/sum;
		}
		yi+=iw;
	}
	delete [] r; delete [] g; delete [] b;
	delete [] r2; delete [] g2; delete [] b2;
}

void CGUIFrameTexturePS2::blur24st(u8 *pix, int x, int y,int w,int h, int imw, int imh)
{

	int sum,cr,cg,cb;
	int read,read3,i,ri,ri3,xl,yl,yi,ym,riw;

	int iw=imw;

	x=txmax(0,x);
	y=txmax(0,y);
	w=x+w-txmax(0,(x+w)-iw);
	h=y+h-txmax(0,(y+h)-imh);
	yi=y*iw;

	for (yl=y;yl<h;yl++)
	{
		for (xl=x;xl<w;xl++)
		{
			cb=cg=cr=sum=0;
			ri=xl-radius;
			for (i=0;i<kernelSize;i++)
			{
				read=ri+i;
				if (read>=x && read<w)
				{
					read+=yi;
					read3 = read*3;
					cr+=mult[i][pix[read3 +0]];
					cg+=mult[i][pix[read3 +1]];
					cb+=mult[i][pix[read3 +2]];
					sum+=kernel[i];
				}
			}
			ri=yi+xl;
			ri3 = ri*3;
			pix[ri3 +0]=cr/sum;
			pix[ri3 +1]=cg/sum;
			pix[ri3 +2]=cb/sum;
		}
		yi+=iw;
	}
	yi=y*iw;

	for (yl=y;yl<h;yl++)
	{
		ym=yl-radius;
		riw=ym*iw;
		for (xl=x;xl<w;xl++)
		{
			cb=cg=cr=sum=0;
			ri=ym;
			read=xl+riw;
			read3 = read*3;
			for (i=0;i<kernelSize;i++)
			{
				if (ri<h && ri>=y)
				{
					cr+=mult[i][pix[read3 +0]];
					cg+=mult[i][pix[read3 +1]];
					cb+=mult[i][pix[read3 +2]];
					sum+=kernel[i];
				}
				ri++;
				read+=iw;
				read3 = read*3;
			}
			int xlyi3 = (xl+yi)*3;
			pix[xlyi3 +0]= cr/sum;
			pix[xlyi3 +1]= cg/sum;
			pix[xlyi3 +2]= cb/sum;
		}
		yi+=iw;
	}
}

#if 0
void CGUIFrameTexturePS2::fastblur24(u8 *pix,int radius, int w, int h)
{

	if (radius<1)
	{
		return;
	}
	int wm=w-1;
	int hm=h-1;

	int wh=w*h;
	u8 *r=new u8[wh];
	u8 *g=new u8[wh];
	u8 *b=new u8[wh];

	int div=radius+radius+1;

	int rsum,gsum,bsum,x,y,i,p1,p2,yp,yi,yi3,yw,ip3,ip13,ip23;
	int *vmin = new int[txmax(w,h)];
	int *vmax = new int[txmax(w,h)];
	int *dv=new int[256*div];

	for (i=0;i<256*div;i++)
	{
		dv[i]=(i/div); 
	}
  
	yw=yi=0;
 
	for (y=0;y<h;y++)
	{
		rsum=gsum=bsum=0;

		for(i=-radius;i<=radius;i++)
		{
			ip3 = (yi+txmin(wm,txmax(i,0)))*3;
			rsum+=pix[ip3 +0];
			gsum+=pix[ip3 +1];
			bsum+=pix[ip3 +2];
		}
		for (x=0;x<w;x++)
		{
			r[yi]=dv[rsum];
			g[yi]=dv[gsum];
			b[yi]=dv[bsum];

			if(y==0)
			{
				vmin[x]=txmin(x+radius+1,wm);
				vmax[x]=txmax(x-radius,0);
			} 
			p1=yw+vmin[x];
			p2=yw+vmax[x];
			ip13=p1*3;
			ip23=p2*3;

			rsum+=pix[ip13+0]-pix[ip23+0];
			gsum+=pix[ip13+1]-pix[ip23+1];
			bsum+=pix[ip13+2]-pix[ip23+2];
			yi++;
		}
		yw+=w;
	}
  
	for (x=0;x<w;x++)
	{
		rsum=gsum=bsum=0;
		yp=-radius*w;
		for(i=-radius;i<=radius;i++)
		{
			yi=txmax(0,yp)+x;
			rsum+=r[yi];
			gsum+=g[yi];
			bsum+=b[yi];
			yp+=w;
		}
		yi=x;
		for (y=0;y<h;y++)
		{
			yi3=yi*3;
			pix[yi3+0]=dv[rsum];
			pix[yi3+1]=dv[gsum];
			pix[yi3+2]=dv[bsum];
			if(x==0)
			{
				vmin[y]=txmin(y+radius+1,hm)*w;
				vmax[y]=txmax(y-radius,0)*w;
			} 
			p1=x+vmin[y];
			p2=x+vmax[y];

			rsum+=r[p1]-r[p2];
			gsum+=g[p1]-g[p2];
			bsum+=b[p1]-b[p2];

			yi+=w;
		}
	}
	delete [] vmin; delete [] vmax; delete [] dv;
	delete [] r; delete [] g; delete [] b;
}
#else
u8 CGUIFrameTexturePS2::dv[256*3];
bool CGUIFrameTexturePS2::done = false;
/*void CGUIFrameTexturePS2::fastblur24(u8 *pix,int radius, int w, int h)
{//radius fixed to 1
	int wm=w-1;
	int hm=h-1;

	int wh=w*h;
	u8 *r=new u8[wh];
	u8 *g=new u8[wh];
	u8 *b=new u8[wh];

	int rsum,gsum,bsum,x,y,i,p1,p2,yp,yi,yi3,yw,ip3,ip13,ip23;
	int *vmin = new int[txmax(w,h)];
	int *vmax = new int[txmax(w,h)];

	if (!done)
	{
		for (i=0;i<256*3;i++)
		{
			dv[i]=(i/3); 
		}
		done = true;
	}
  
	yw=yi=0;
 
	for (y=0;y<h;y++)
	{
		rsum=gsum=bsum=0;

		for(i=-1;i<=1;i++)
		{
			ip3 = (yi+txmin(wm,txmax(i,0))); ip3 += ip3<<1;
			rsum+=pix[ip3 +0];
			gsum+=pix[ip3 +1];
			bsum+=pix[ip3 +2];
		}
		for (x=0;x<w;x++)
		{
			r[yi]=dv[rsum];
			g[yi]=dv[gsum];
			b[yi]=dv[bsum];

			if(y==0)
			{
				vmin[x]=txmin(x+1+1,wm);
				vmax[x]=txmax(x-1,0);
			} 
			p1=yw+vmin[x];
			p2=yw+vmax[x];
			ip13=(p1<<1) + p1;
			ip23=(p2<<1) + p2;

			rsum+=pix[ip13+0]-pix[ip23+0];
			gsum+=pix[ip13+1]-pix[ip23+1];
			bsum+=pix[ip13+2]-pix[ip23+2];
			yi++;
		}
		yw+=w;
	}
  
	for (x=0;x<w;x++)
	{
		rsum=gsum=bsum=0;
		yp=-1*w;
		for(i=-1;i<=1;i++)
		{
			yi=txmax(0,yp)+x;
			rsum+=r[yi];
			gsum+=g[yi];
			bsum+=b[yi];
			yp+=w;
		}
		yi=x;
		for (y=0;y<h;y++)
		{
			yi3=(yi<<1)+yi;
			pix[yi3+0]=dv[rsum];
			pix[yi3+1]=dv[gsum];
			pix[yi3+2]=dv[bsum];
			if(x==0)
			{
				vmin[y]=txmin(y+1+1,hm)*w;
				vmax[y]=txmax(y-1,0)*w;
			} 
			p1=x+vmin[y];
			p2=x+vmax[y];

			rsum+=r[p1]-r[p2];
			gsum+=g[p1]-g[p2];
			bsum+=b[p1]-b[p2];

			yi+=w;
		}
	}
	delete [] vmin; delete [] vmax;
	delete [] r; delete [] g; delete [] b;
}*/
void CGUIFrameTexturePS2::fastblur24(u8 *pix,int radius, int w, int h)
{//radius fixed to 1
	if (!done)
	{
		for (int i=0;i<256*3;i++)
		{
			dv[i]=(i/3); 
		}
		done = true;
	}
	int linesize = w + (w<<1);

	for (int y = 0, linepos = 0; y < h; y++, linepos += linesize)
	{
		u8 *tmpPix = pix + linepos;
		int sumr = tmpPix[0] + tmpPix[3];
		int sumg = tmpPix[1] + tmpPix[4];
		int sumb = tmpPix[2] + tmpPix[5];
		u8 firstR = tmpPix[0];
		u8 firstG = tmpPix[1];
		u8 firstB = tmpPix[2];
		tmpPix[0] = sumr>>1; tmpPix[1] = sumg>>1; tmpPix[2] = sumb>>1;
		tmpPix += 3;
		for (int x = 1; x < w-1; x++)
		{
			sumr = firstR + tmpPix[0] + tmpPix[3];
			sumg = firstG + tmpPix[1] + tmpPix[4];
			sumb = firstB + tmpPix[2] + tmpPix[5];
			firstR = tmpPix[0];
			firstG = tmpPix[1];
			firstB = tmpPix[2];
			tmpPix[0] = dv[sumr]; tmpPix[1] = dv[sumg]; tmpPix[2] = dv[sumb];
			tmpPix += 3;
		}
		sumr = firstR + tmpPix[0];
		sumg = firstG + tmpPix[1];
		sumb = firstB + tmpPix[2];
		tmpPix[0] = sumr>>1; tmpPix[1] = sumg>>1; tmpPix[2] = sumb>>1;
	}


	for (int x = 0; x < w; x++)
	{
		u8 *tmpPix = pix + x + (x<<1);
		int sumr = tmpPix[0] + tmpPix[0+linesize];
		int sumg = tmpPix[1] + tmpPix[1+linesize];
		int sumb = tmpPix[2] + tmpPix[2+linesize];
		u8 firstR = tmpPix[0];
		u8 firstG = tmpPix[1];
		u8 firstB = tmpPix[2];
		tmpPix[0] = sumr>>1; tmpPix[1] = sumg>>1; tmpPix[2] = sumb>>1;
		tmpPix += linesize;
		for (int y = 1; y < h-1; y++)
		{
			sumr = firstR + tmpPix[0] + tmpPix[0+linesize];
			sumg = firstG + tmpPix[1] + tmpPix[1+linesize];
			sumb = firstB + tmpPix[2] + tmpPix[2+linesize];
			firstR = tmpPix[0];
			firstG = tmpPix[1];
			firstB = tmpPix[2];
			tmpPix[0] = dv[sumr]; tmpPix[1] = dv[sumg]; tmpPix[2] = dv[sumb];
			tmpPix += linesize;
		}
		sumr = firstR + tmpPix[0];
		sumg = firstG + tmpPix[1];
		sumb = firstB + tmpPix[2];
		tmpPix[0] = sumr>>1; tmpPix[1] = sumg>>1; tmpPix[2] = sumb>>1;
	}
}
#endif
std::list<CGUIFrameTexturePS2*> CGUIFrameTexturePS2::m_tex_list;// = std::list<CGUIFrameTexturePS2*>();

int CGUIFrameTexturePS2::radius = 0;
int CGUIFrameTexturePS2::kernelSize = 0;
int CGUIFrameTexturePS2::kernel[TX_GAUSSIAN_RADIUS*2 +1];
int CGUIFrameTexturePS2::mult[TX_GAUSSIAN_RADIUS*2 +1][256];
