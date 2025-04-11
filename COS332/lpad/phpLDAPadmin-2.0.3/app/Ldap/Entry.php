<?php

namespace App\Ldap;

use Illuminate\Support\Collection;
use Illuminate\Support\Facades\Crypt;
use LdapRecord\Support\Arr;
use LdapRecord\Models\Model;
use LdapRecord\Query\Model\Builder;

use App\Classes\LDAP\Attribute;
use App\Classes\LDAP\Attribute\Factory;
use App\Classes\LDAP\Export\LDIF;
use App\Exceptions\Import\AttributeException;
use App\Exceptions\InvalidUsage;

class Entry extends Model
{
	// Our Attribute objects
	private Collection $objects;
	/* @deprecated */
	private bool $noObjectAttributes = FALSE;
	// For new entries, this is the container that this entry will be stored in
	private string $rdnbase;

	/* OVERRIDES */

	public function __construct(array $attributes = [])
	{
		$this->objects = collect();

		parent::__construct($attributes);
	}

	public function discardChanges(): static
	{
		parent::discardChanges();

		// If we are discharging changes, we need to reset our $objects;
		$this->objects = $this->getAttributesAsObjects();

		return $this;
	}

	/**
	 * This function overrides getAttributes to use our collection of Attribute objects instead of the models attributes.
	 *
	 * @return array
	 * @note $this->attributes may not be updated with changes
	 */
	public function getAttributes(): array
	{
		return $this->objects
			->map(fn($item)=>$item->values)
			->toArray();
	}

	/**
	 * Determine if the new and old values for a given key are equivalent.
	 *
	 * @todo This function barfs on language tags, eg: key = givenname;lang-ja
	 */
	protected function originalIsEquivalent(string $key): bool
	{
		$key = $this->normalizeAttributeKey($key);

		// @todo Silently ignore keys of language tags - we should work with them
		if (str_contains($key,';'))
			return TRUE;

		return ((! array_key_exists($key,$this->original)) && (! $this->objects->has($key)))
			|| (! $this->getObject($key)->isDirty());
	}

	public static function query(bool $noattrs=false): Builder
	{
		$o = new static;

		if ($noattrs)
			$o->noObjectAttributes();

		return $o->newQuery();
	}

	/**
	 * As attribute values are updated, or new ones created, we need to mirror that
	 * into our $objects. This is called when we $o->key = $value
	 *
	 * @param string $key
	 * @param mixed $value
	 * @return $this
	 */
	public function setAttribute(string $key,mixed $value): static
	{
		parent::setAttribute($key,$value);

		$key = $this->normalizeAttributeKey($key);

		$o = $this->objects->get($key) ?: Factory::create($this->dn ?: '',$key,[],Arr::get($this->attributes,'objectclass',[]));
		$o->values = collect($this->attributes[$key]);

		$this->objects->put($key,$o);

		return $this;
	}

	/**
	 * We'll shadow $this->attributes to $this->objects - a collection of Attribute objects
	 *
	 * Using the objects, it'll make it easier to work with attribute values
	 *
	 * @param array $attributes
	 * @return $this
	 */
	public function setRawAttributes(array $attributes = []): static
	{
		parent::setRawAttributes($attributes);

		// We only set our objects on DN entries (otherwise we might get into a recursion loop if this is the schema DN)
		if ($this->dn && (! in_array($this->dn,Arr::get($this->attributes,'subschemasubentry',[])))) {
			$this->objects = $this->getAttributesAsObjects();

		} else {
			$this->objects = collect();
		}

		return $this;
	}

	/* ATTRIBUTES */

	/**
	 * Return a key to use for sorting
	 *
	 * @return string
	 */
	public function getSortKeyAttribute(): string
	{
		return collect(explode(',',$this->getDn()))->reverse()->join(',');
	}

	/* METHODS */

	/**
	 * Add an attribute to this entry, if the attribute already exists, then we'll add the value to the existing item.
	 *
	 * This is primarily used by LDIF imports, where attributes have multiple entries over multiple lines
	 *
	 * @param string $key
	 * @param mixed $value
	 * @return void
	 * @throws AttributeException
	 * @note Attributes added this way dont have objectclass information, and the Model::attributes are not populated
	 */
	public function addAttributeItem(string $key,mixed $value): void
	{
		// While $value is mixed, it can only be a string
		if (! is_string($value))
			throw new \Exception('value should be a string');

		$key = $this->normalizeAttributeKey($key);

		if (! config('server')->schema('attributetypes')->has($key))
			throw new AttributeException(sprintf('Schema doesnt have attribute [%s]',$key));

		$o = $this->objects->get($key) ?: Attribute\Factory::create($this->dn ?: '',$key,[]);
		$o->addValue($value);

		$this->objects->put($key,$o);
	}

	/**
	 * Convert all our attribute values into an array of Objects
	 *
	 * @return Collection
	 */
	private function getAttributesAsObjects(): Collection
	{
		$result = collect();
		$entry_oc = Arr::get($this->attributes,'objectclass',[]);

		foreach ($this->attributes as $attribute => $values) {
			// If the attribute name has tags
			$matches = [];
			if (preg_match('/^([a-zA-Z]+)(;([a-zA-Z-;]+))+/',$attribute,$matches)) {
				$attribute = $matches[1];

				// If the attribute doesnt exist we'll create it
				$o = Arr::get(
					$result,
					$attribute,
					Factory::create(
						$this->dn,
						$attribute,
						Arr::get($this->original,$attribute,[]),
						$entry_oc,
					));
				$o->setLangTag($matches[3],$values);

			} else {
				$o = Factory::create($this->dn,$attribute,Arr::get($this->original,$attribute,[]),$entry_oc);
			}

			if (! $result->has($attribute)) {
				// Store our new values to know if this attribute has changed
				$o->values = collect($values);

				$result->put($attribute,$o);
			}
		}

		$sort = collect(config('pla.attr_display_order',[]))->map(fn($item)=>strtolower($item));

		// Order the attributes
		return $result->sortBy([function(Attribute $a,Attribute $b) use ($sort): int {
			if ($a === $b)
				return 0;

			// Check if $a/$b are in the configuration to be sorted first, if so get it's key
			$a_key = $sort->search($a->name_lc);
			$b_key = $sort->search($b->name_lc);

			// If the keys were not in the sort list, set the key to be the count of elements (ie: so it is last to be sorted)
			if ($a_key === FALSE)
				$a_key = $sort->count()+1;

			if ($b_key === FALSE)
				$b_key = $sort->count()+1;

			// Case where neither $a, nor $b are in pla.attr_display_order, $a_key = $b_key = one greater than num elements.
			// So we sort them alphabetically
			if ($a_key === $b_key)
				return strcasecmp($a->name,$b->name);

			// Case where at least one attribute or its friendly name is in $attrs_display_order
			// return -1 if $a before $b in $attrs_display_order
			return ($a_key < $b_key) ? -1 : 1;
		}]);
	}

	/**
	 * Return a list of available attributes - as per the objectClass entry of the record
	 *
	 * @return Collection
	 */
	public function getAvailableAttributes(): Collection
	{
		$result = collect();

		foreach ($this->objectclass as $oc)
			$result = $result->merge(config('server')->schema('objectclasses',$oc)->attributes);

		return $result;
	}

	/**
	 * Return a secure version of the DN
	 * @param string $cmd
	 * @return string
	 */
	public function getDNSecure(string $cmd=''): string
	{
		return Crypt::encryptString(($cmd ? sprintf('*%s|',$cmd) : '').$this->getDn());
	}

	/**
	 * Return a list of LDAP internal attributes
	 *
	 * @return Collection
	 */
	public function getInternalAttributes(): Collection
	{
		return $this->objects
			->filter(fn($item)=>$item->is_internal);
	}

	/**
	 * Get an attribute as an object
	 *
	 * @param string $key
	 * @return Attribute|null
	 */
	public function getObject(string $key): Attribute|null
	{
		return match ($key) {
			'rdn' => $this->getRDNObject(),

			default => $this->objects
				->get($this->normalizeAttributeKey($key))
		};
	}

	public function getObjects(): Collection
	{
		// In case we havent built our objects yet (because they werent available while determining the schema DN)
		if ((! $this->objects->count()) && $this->attributes)
			$this->objects = $this->getAttributesAsObjects();

		return $this->objects;
	}

	/**
	 * Return a list of attributes without any values
	 *
	 * @return Collection
	 */
	public function getMissingAttributes(): Collection
	{
		return $this->getAvailableAttributes()
			->filter(fn($a)=>(! $this->getVisibleAttributes()->contains(fn($b)=>($a->name === $b->name))));
	}

	private function getRDNObject(): Attribute\RDN
	{
		$o = new Attribute\RDN('','dn',['']);
		// @todo for an existing object, rdnbase would be null, so dynamically get it from the DN.
		$o->setBase($this->rdnbase);
		$o->setAttributes($this->getAvailableAttributes()->filter(fn($item)=>$item->required));

		return $o;
	}

	/**
	 * Return this list of user attributes
	 *
	 * @return Collection
	 */
	public function getVisibleAttributes(): Collection
	{
		return $this->objects
			->filter(fn($item)=>! $item->is_internal);
	}

	public function hasAttribute(int|string $key): bool
	{
		return $this->objects
			->has($key);
	}

	/**
	 * Export this record
	 *
	 * @param string $method
	 * @param string $scope
	 * @return string
	 * @throws \Exception
	 */
	public function export(string $method,string $scope): string
	{
		// @todo To implement
		switch ($scope) {
			case 'base':
			case 'one':
			case 'sub':
				break;

			default:
				throw new \Exception('Export scope unknown:'.$scope);
		}

		switch ($method) {
			case 'ldif':
				return new LDIF(collect($this));

			default:
				throw new \Exception('Export method not implemented:'.$method);
		}
	}

	/**
	 * Return an icon for a DN based on objectClass
	 *
	 * @return string
	 */
	public function icon(): string
	{
		$objectclasses = array_map('strtolower',$this->objectclass);

		// Return icon based upon objectClass value
		if (in_array('person',$objectclasses) ||
			in_array('organizationalperson',$objectclasses) ||
			in_array('inetorgperson',$objectclasses) ||
			in_array('account',$objectclasses) ||
			in_array('posixaccount',$objectclasses))

			return 'fas fa-user';

		elseif (in_array('organization',$objectclasses))
			return 'fas fa-university';

		elseif (in_array('organizationalunit',$objectclasses))
			return 'fas fa-object-group';

		elseif (in_array('posixgroup',$objectclasses) ||
			in_array('groupofnames',$objectclasses) ||
			in_array('groupofuniquenames',$objectclasses) ||
			in_array('group',$objectclasses))

			return 'fas fa-users';

		elseif (in_array('dcobject',$objectclasses) ||
			in_array('domainrelatedobject',$objectclasses) ||
			in_array('domain',$objectclasses) ||
			in_array('builtindomain',$objectclasses))

			return 'fas fa-network-wired';

		elseif (in_array('alias',$objectclasses))
			return 'fas fa-theater-masks';

		elseif (in_array('country',$objectclasses))
			return sprintf('flag %s',strtolower(Arr::get($this->c,0)));

		elseif (in_array('device',$objectclasses))
			return 'fas fa-mobile-alt';

		elseif (in_array('document',$objectclasses))
			return 'fas fa-file-alt';

		elseif (in_array('iphost',$objectclasses))
			return 'fas fa-wifi';

		elseif (in_array('room',$objectclasses))
			return 'fas fa-door-open';

		elseif (in_array('server',$objectclasses))
			return 'fas fa-server';

		elseif (in_array('openldaprootdse',$objectclasses))
			return 'fas fa-info';

		// Default
		return 'fa-fw fas fa-cog';
	}

	/**
	 * Dont convert our $this->attributes to $this->objects when creating a new Entry::class
	 *
	 * @return $this
	 * @deprecated
	 */
	public function noObjectAttributes(): static
	{
		$this->noObjectAttributes = TRUE;

		return $this;
	}

	public function setRDNBase(string $bdn): void
	{
		if ($this->exists)
			throw new InvalidUsage('Cannot set RDN base on existing entries');

		$this->rdnbase = $bdn;
	}
}