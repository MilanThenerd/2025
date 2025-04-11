<?php

namespace App\Classes\LDAP\Attribute\Binary;

use Illuminate\Contracts\View\View;

use App\Classes\LDAP\Attribute\Binary;
use App\Traits\MD5Updates;

/**
 * Represents an JpegPhoto Attribute
 */
final class JpegPhoto extends Binary
{
	use MD5Updates;

	public function render(bool $edit=FALSE,bool $old=FALSE,bool $new=FALSE): View
	{
		return view('components.attribute.binary.jpegphoto')
			->with('o',$this)
			->with('edit',$edit)
			->with('old',$old)
			->with('new',$new)
			->with('f',new \finfo);
	}
}