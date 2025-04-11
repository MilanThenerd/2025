<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Arr;
use Illuminate\Support\Facades\Crypt;
use Illuminate\Support\Facades\Log;
use Illuminate\Support\Collection;

use App\Classes\LDAP\Server;

class APIController extends Controller
{
	/**
	 * Get the LDAP server BASE DNs
	 *
	 * @return Collection
	 * @throws \LdapRecord\Query\ObjectNotFoundException
	 */
	public function bases(): Collection
	{
		$base = Server::baseDNs() ?: collect();

		return $base
			->transform(fn($item)=>
				[
					'title'=>$item->getRdn(),
					'item'=>$item->getDNSecure(),
					'lazy'=>TRUE,
					'icon'=>'fa-fw fas fa-sitemap',
					'tooltip'=>$item->getDn(),
				]);
	}

	/**
	 * @param Request $request
	 * @return Collection
	 */
	public function children(Request $request): Collection
	{
		$dn = Crypt::decryptString($request->query('key'));

		// Sometimes our key has a command, so we'll ignore it
		if (str_starts_with($dn,'*') && ($x=strpos($dn,'|')))
			$dn = substr($dn,$x+1);

		Log::debug(sprintf('%s: Query [%s]',__METHOD__,$dn));

		return (config('server'))
			->children($dn)
			->transform(fn($item)=>
				[
					'title'=>$item->getRdn(),
					'item'=>$item->getDNSecure(),
					'icon'=>$item->icon(),
					'lazy'=>Arr::get($item->getAttribute('hassubordinates'),0) == 'TRUE',
					'tooltip'=>$item->getDn(),
				])
			->prepend(
				[
					'title'=>sprintf('[%s]',__('Create Entry')),
					'item'=>Crypt::encryptString(sprintf('*%s|%s','create',$dn)),
					'lazy'=>FALSE,
					'icon'=>'fas fa-fw fa-square-plus text-warning',
					'tooltip'=>__('Create new LDAP item here'),
				]);
	}

	public function schema_view(Request $request)
	{
		$server = new Server;

		switch($request->type) {
			case 'objectclasses':
				return view('fragment.schema.objectclasses')
					->with('objectclasses',$server->schema('objectclasses')->sortBy(fn($item)=>strtolower($item->name)));

			case 'attributetypes':
				return view('fragment.schema.attributetypes')
					->with('server',$server)
					->with('attributetypes',$server->schema('attributetypes')->sortBy(fn($item)=>strtolower($item->name)));

			case 'ldapsyntaxes':
				return view('fragment.schema.ldapsyntaxes')
					->with('ldapsyntaxes',$server->schema('ldapsyntaxes')->sortBy(fn($item)=>strtolower($item->description)));

			case 'matchingrules':
				return view('fragment.schema.matchingrules')
					->with('matchingrules',$server->schema('matchingrules')->sortBy(fn($item)=>strtolower($item->name)));

			default:
				abort(404);
		}
	}

	/**
	 * Return the required and additional attributes for an object class
	 *
	 * @param Request $request
	 * @param string $objectclass
	 * @return array
	 */
	public function schema_objectclass_attrs(string $objectclass): array
	{
		$oc = config('server')->schema('objectclasses',$objectclass);

		return [
			'must' => $oc->getMustAttrs()->pluck('name'),
			'may' => $oc->getMayAttrs()->pluck('name'),
		];
	}
}