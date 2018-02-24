#include <gameboy.hpp>
#include <globals.hpp>
#include <thread>

void		GameboyCpu::dma(void)
{
	size_t	blksize;

	if (this->_doDma)
	{
		if (!this->_dma_src)
		{
			*(gbram + 0xFF41) &= ~3;
			this->_dma_src = *(gbram + 0xFF46) << 8;
			this->_dma_dst = 0xFE00;
		}

		if (this->_dma_src > 0xF100)
			return ;

		blksize = this->_dma_src + this->_dma_size <= this->_dma_dst ? 
					this->_dma_size : this->_dma_dst - this->_dma_size;
		memcpy(gbram + this->_dma_src, gbram + this->_dma_dst, blksize);
		this->_dma_src += blksize;
		this->_dma_dst += blksize;
		if (this->_dma_src >= this->_dma_dst)
		{
			*(gbram + 0xFF41) |= ~1;
			this->_doDma = false;
			this->_dma_src = 0;
			this->_dma_dst = 0;
		}
	}
}
