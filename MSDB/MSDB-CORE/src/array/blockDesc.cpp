#include <array/blockDesc.h>

namespace msdb
{
const blockSize INVALID_BLOCK_SIZE = static_cast<blockSize>(~0);
const blockId INVALID_BLOCK_ID = static_cast<blockId>(~0);

blockDesc::blockDesc()
{
	// TODO::initialization
}

blockDesc::blockDesc(const blockId id, const dimension& dims, const coor sp, const coor ep, const blockSize mSize)
	: id_(id), dims_(dims), sp_(sp), ep_(ep), mSize_(mSize)
{
}
void blockDesc::initBlockCoor()
{
	for (dimensionId d = 0; d < this->dims_.size(); ++d)
	{
		// this->ep_[d] - this->sp_[d] = block width
		this->blockCoor_[d] = this->sp_[d] / (this->ep_[d] - this->sp_[d]);
	}
}
}	// msdb